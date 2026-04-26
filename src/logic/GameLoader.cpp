#include "logic/GameLoader.hpp"

#include <memory>

#include "core/Property.hpp"
#include "core/Tiles.hpp"
#include "core/card/Card.hpp"
#include "core/card/CardDeck.hpp"
#include "core/card/DemolitionCard.hpp"
#include "core/card/DiscountCard.hpp"
#include "core/card/LassoCard.hpp"
#include "core/card/MoveCard.hpp"
#include "core/card/ShieldCard.hpp"
#include "core/card/TeleportCard.hpp"
#include "core/player/Player.hpp"
#include "data/TransactionLogger.hpp"
#include "logic/Board.hpp"
#include "logic/Game.hpp"

namespace logic {

static std::unique_ptr<core::ActionCard> buildCardFromDTO(
    const data::HeldCardDTO& dto) {
  if (dto.cardType == "MoveCard") {
    int steps = (dto.value > 0) ? dto.value : 1;
    return core::MoveCard::make(steps,
                                "Maju " + std::to_string(steps) + " petak.");
  }
  if (dto.cardType == "ShieldCard") {
    return core::ShieldCard::make(
        "Kebal tagihan atau sanksi selama 1 giliran.");
  }
  if (dto.cardType == "DiscountCard") {
    // value disimpan sebagai persentase bulat (misal 30 = 30%)
    float rate =
        (dto.value > 0) ? static_cast<float>(dto.value) / 100.0f : 0.10f;
    return std::unique_ptr<core::ActionCard>(
        new core::DiscountCard(rate, "Diskon pembelian properti."));
  }
  if (dto.cardType == "TeleportCard") {
    return core::TeleportCard::make(0, "Pindah ke petak manapun.");
  }
  if (dto.cardType == "LassoCard") {
    return core::LassoCard::make("Tarik lawan ke posisimu.");
  }
  if (dto.cardType == "DemolitionCard") {
    return core::DemolitionCard::make("Hancurkan bangunan lawan.");
  }
  // Fallback: jenis tidak dikenal, kembalikan nullptr (diabaikan oleh caller)
  return nullptr;
}

void GameLoader::applyDTO(const data::GameStateDTO& dto, Game& game,
                          const std::string& configPath) {
  // Step 1: rebuild board dari config sesuai boardSize yang tersimpan di save
  game.initialize(dto.boardSize, configPath);

  // Step 2-5: restore runtime state
  restoreGameMeta(dto, game);
  restorePlayers(dto, game);
  restoreProperties(dto, game);
  restoreDeck(dto, game);
}

void GameLoader::restoreGameMeta(const data::GameStateDTO& dto, Game& game) {
  game.setTurnCount(dto.turnCount);
  game.setMaxTurn(dto.maxTurn);
  game.setTurnOrder(dto.turnOrder);

  int idx = 0;
  for (int i = 0; i < static_cast<int>(dto.turnOrder.size()); ++i) {
    if (dto.turnOrder[i] == dto.activePlayerName) {
      idx = i;
      break;
    }
  }
  game.setCurrentPlayerIdx(idx);
  data::TransactionLogger::get().restore(dto.log);
}

void GameLoader::restorePlayers(const data::GameStateDTO& dto, Game& game) {
  Board& board = game.getBoard();

  for (const auto& pDTO : dto.players) {
    core::Player* player = game.getPlayerByName(pDTO.name);
    if (!player) continue;

    // Balance, setBalance dipakai agar tidak menambah ke nilai default
    player->setBalance(pDTO.balance);

    // Position, resolve dari code tile ke 0-indexed board index
    core::Tile* tile = board.getTileByCode(pDTO.positionCode);
    if (tile) player->setPosition(tile->getPosition());

    // Status jail / bankrupt
    if (pDTO.isBankrupt) {
      player->setBankrupted(true);
    } else if (pDTO.jailTurns > 0) {
      player->goToJail();
      for (int i = 0; i < pDTO.jailTurns; ++i) player->incrementJailTurns();
    }

    // Kartu tangan, direstorasi AKURAT dari HeldCardDTO
    for (const auto& cardDTO : pDTO.heldCards) {
      auto card = buildCardFromDTO(cardDTO);
      if (card) {
        // CardDeck memiliki kartu; player menyimpan pointer non-owning.
        // Kartu tangan disimpan sementara di skillDeck_ agar lifetime
        // dikelola oleh deck, ini tradeoff pragmatis sampai ada
        // dedicated hand-card storage dengan ownership yang jelas.
        // TODO: pertimbangkan dedicated storage untuk kartu tangan
        //       agar terpisah dari draw deck.
        core::ActionCard* rawPtr = card.get();
        game.getSkillDeck().addCard(std::move(card));
        player->addCard(rawPtr);
      }
    }
  }
}

void GameLoader::restoreProperties(const data::GameStateDTO& dto, Game& game) {
  Board& board = game.getBoard();

  for (const auto& propDTO : dto.properties) {
    const auto* dtoPtr = propDTO.get();
    if (!dtoPtr) continue;

    core::Tile* tile = board.getTileByCode(dtoPtr->code);
    if (!tile) continue;

    if (tile->getType() != core::TileType::PROPERTY) continue;
    auto* propertyTile = static_cast<core::PropertyTile*>(tile);
    core::Property& prop = propertyTile->getProperty();

    // 1. Set owner DULU, mortgage() butuh isOwned() == true
    if (!dtoPtr->ownerName.empty()) {
      core::Player* owner = game.getPlayerByName(dtoPtr->ownerName);
      if (owner) {
        prop.setOwner(owner);
        owner->addProperty(prop);
      }
    }

    // 2. Mortgage status
    if (dtoPtr->isMortgaged && prop.isOwned()) prop.mortgage();

    // 3. Street-specific: building level + festival
    if (dtoPtr->getType() == "street" &&
        prop.getType() == core::PropertyTileType::STREET) {
      const auto* sDTO = static_cast<const data::StreetPropertyDTO*>(dtoPtr);
      auto* street = static_cast<core::Street*>(&prop);
      street->setLevel(sDTO->level);
      if (sDTO->festMult != 1 || sDTO->festDur != 0)
        prop.setFestivalState(sDTO->festMult, sDTO->festDur);
    }
  }
}

void GameLoader::restoreDeck(const data::GameStateDTO& dto, Game& game) {
  if (dto.skillDeckOrder.empty()) return;

  std::vector<std::unique_ptr<core::ActionCard>> newDeckCards;
  newDeckCards.reserve(dto.skillDeckOrder.size());

  for (const auto& cardType : dto.skillDeckOrder) {
    data::HeldCardDTO dummy;
    dummy.cardType = cardType;
    dummy.value = 0;  // random akan di-generate oleh factory

    // Untuk MoveCard dan DiscountCard, nilai di deck di-random ulang
    auto card = buildCardFromDTO(dummy);
    if (card) newDeckCards.push_back(std::move(card));
  }

  // Ganti isi skill deck dengan urutan yang direstorasi (tanpa shuffle ulang)
  game.getSkillDeck() =
      core::CardDeck<core::ActionCard>(std::move(newDeckCards));
}

}  // namespace logic