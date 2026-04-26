#include "logic/Game.hpp"

#include <random>

#include "core/GameException.hpp"
#include "core/Property.hpp"
#include "core/Tiles.hpp"
#include "core/card/ChanceCard.hpp"
#include "core/card/CommunityChestCard.hpp"
#include "core/card/DemolitionCard.hpp"
#include "core/card/DiscountCard.hpp"
#include "core/card/LassoCard.hpp"
#include "core/card/MoveCard.hpp"
#include "core/card/ShieldCard.hpp"
#include "core/card/TeleportCard.hpp"
#include "core/player/Player.hpp"
#include "data/ConfigReader.hpp"
#include "data/TransactionLogger.hpp"
#include "logic/Auction.hpp"
#include "logic/UIInputMediator.hpp"

namespace logic {

Game::Game(std::vector<core::Player*> players)
    : players_(std::move(players)),
      mediator_(nullptr),
      currentPlayerId_(0),
      state_(GameState::PRE_ROLL),
      lastDiceRoll_({0, 0}),
      turnCount_(1),
      rng_(std::random_device{}()),
      doubles_(0),
      hasExtraTurn_(false) {}

void Game::initialize(int boardSize, const std::string& configPath) {
  boardSize_ = boardSize;
  configPath_ = configPath;

  board_.clear();

  data::ConfigReader::initialize(configPath, boardSize);
  auto& reader = data::ConfigReader::get();

  auto specialCfg = reader.readSpecial();
  auto miscCfg = reader.readMisc();
  auto railRents = reader.readRailroadRents();
  auto utilMults = reader.readUtilityMultipliers();

  auto entries = reader.buildBoard();

  for (auto& entry : entries) {
    board_.addTile(entry.releaseTile(), entry.getCode());
  }

  core::Utility::setMultiplierTabel(utilMults);
  core::Railroad::setRentTable(railRents);

  goSalary_ = specialCfg.goSalary;
  jailFine_ = specialCfg.jailFine;
  maxTurn_ = miscCfg.maxTurn;

  for (auto* p : players_) {
    if (p) p->setBalance(miscCfg.initialBalance);
  }

  buildChanceDeck();
  buildCommunityChestDeck();
  buildSkillDeck();
}

void Game::startGame() { state_ = GameState::PRE_ROLL; }

Board& Game::getBoard() { return board_; }

void Game::setMediator(UIInputMediator* mediator) { mediator_ = mediator; }

void Game::nextTurn() {
  if (checkWinCondition()) {
    state_ = GameState::GAME_OVER;
    return;
  }

  if (!hasExtraTurn_) {
    doubles_ = 0;

    do {
      currentPlayerId_ = (currentPlayerId_ + 1) % players_.size();
      if (currentPlayerId_ == 0) {
        turnCount_++;
      }
    } while (players_[currentPlayerId_]->isBankrupted());
  }

  if (auto* incoming = players_[currentPlayerId_]) {
    incoming->resetPerTurnFlags();
  }

  hasExtraTurn_ = false;
  state_ = GameState::PRE_ROLL;
}

bool Game::checkWinCondition() const {
  int activePlayers = 0;
  for (const auto* p : players_) {
    if (!p->isBankrupted()) {
      activePlayers++;
    }
  }
  return activePlayers <= 1;
}

void Game::rollDice() {
  if (state_ != GameState::PRE_ROLL && state_ != GameState::WAITING_FOR_DICE) {
    throw InvalidStateException("rollDice",
                                std::to_string(static_cast<int>(state_)));
  }

  lastDiceRoll_.first = rng_.operator()() % 6 + 1;
  lastDiceRoll_.second = rng_.operator()() % 6 + 1;

  if (lastDiceRoll_.first == lastDiceRoll_.second) {
    doubles_++;
    if (doubles_ >= 3) {
      core::Player* p = getCurrentPlayer();
      p->goToJail();
      doubles_ = 0;
      hasExtraTurn_ = false;
    } else {
      hasExtraTurn_ = true;
    }
  } else {
    doubles_ = 0;
    hasExtraTurn_ = false;
  }

  logEvent(data::LogAction::DICE_ROLL, *getCurrentPlayer(),
           lastDiceRoll_.first + lastDiceRoll_.second);
}

void Game::moveCurrentPlayer() {
  core::Player* p = getCurrentPlayer();

  if (p->isInJail()) {
    return;
  }

  int steps = lastDiceRoll_.first + lastDiceRoll_.second;
  int boardSize = board_.getTileCount();

  if (boardSize == 0) {
    throw InvalidConfigException("Board", "at least 1 tile");
  }

  int currentPos = p->getPosition();
  int newPos = (currentPos + steps) % boardSize;

  stepThrough(*p, currentPos, newPos, /*firePassed=*/true);
  logEvent(data::LogAction::PIECE_MOVEMENT, *p, newPos);
}

void Game::stepThrough(core::Player& p, int from, int to, bool firePassed) {
  int boardSize = board_.getTileCount();
  if (boardSize <= 0) {
    throw InvalidConfigException("Board", "at least 1 tile");
  }

  int dest = ((to % boardSize) + boardSize) % boardSize;
  int origin = ((from % boardSize) + boardSize) % boardSize;

  if (firePassed && origin != dest) {
    int idx = origin;
    while (true) {
      idx = (idx + 1) % boardSize;
      if (idx == dest) break;
      core::Tile* crossed = board_.getTile(idx);
      if (crossed) crossed->onPassed(p, *this);
    }
  }

  p.setPosition(dest);
  core::Tile* landedTile = board_.getTile(dest);
  handleTileAction(landedTile);
}

void Game::handleTileAction(core::Tile* tile) {
  if (!tile) throw InvalidMoveException("invalid tile.");

  state_ = GameState::TILE_ACTION;
  core::Player* p = getCurrentPlayer();

  tile->onLanded(*p, *this);
}

void Game::buyProperty(core::Property& prop) {
  core::Player* p = getCurrentPlayer();
  int price = prop.getPrice();

  if (prop.getOwner() != nullptr) {
    throw InvalidMoveException("owned property exception");
  }

  if (!p->canAfford(price)) {
    throw InsufficientFundsException(p->getBalance(), price);
  }

  *p -= price;
  bank_.receive(price);

  prop.setOwner(p);
  p->addProperty(prop);
  logEvent(data::LogAction::PROPERTY_PURCHASE, *p, prop, price);
}

void Game::buildHouse(core::Player* buyer, core::Tile* at) {
  if (at == nullptr) {
    throw InvalidPropertyTypeException("unknown", "bukan properti");
  }
  if (at->getType() != core::TileType::PROPERTY) {
    throw InvalidPropertyTypeException(at->getName(), "bukan properti");
  }
  auto* propertyTile = static_cast<core::PropertyTile*>(at);
  core::Property& prop = propertyTile->getProperty();

  if (prop.getType() != core::PropertyTileType::STREET) {
    throw InvalidPropertyTypeException(prop.getName(), "bukan Street");
  }

  core::Street* street = static_cast<core::Street*>(&prop);

  if (street->getOwner() != buyer) {
    throw UnauthorizedActionException(
        buyer->getName(),
        street->getOwner() ? street->getOwner()->getName() : "none",
        street->getName());
  }

  int cost = street->getHouseCost();
  if (!buyer->canAfford(cost)) {
    throw InsufficientFundsException(buyer->getBalance(), cost);
  }

  *buyer -= cost;
  bank_.receive(cost);
  street->build();
  logEvent(data::LogAction::BUILD_HOUSE, *buyer, *street, cost);
}

void Game::sellHouse(core::Player* seller, core::Tile* at) {
  if (at == nullptr || at->getType() != core::TileType::PROPERTY) {
    throw InvalidMoveException("no house exception.");
  }
  auto* propertyTile = static_cast<core::PropertyTile*>(at);
  core::Property& prop = propertyTile->getProperty();

  if (prop.getType() != core::PropertyTileType::STREET) {
    throw InvalidMoveException("no house exception.");
  }

  core::Street* street = static_cast<core::Street*>(&prop);

  if (street->getHouseCount() == 0 && street->getHotelCount() == 0) {
    throw InvalidMoveException("no building exception");
  }

  if (street->getOwner() != seller) {
    throw UnauthorizedActionException(
        seller->getName(),
        street->getOwner() ? street->getOwner()->getName() : "none",
        street->getName());
  }

  int refund = street->getHouseCost() / 2;

  street->demolish(1);
  bank_.pay(*seller, refund);
  logEvent(data::LogAction::SALE_HOUSE, *seller, *street, refund);
}

void Game::mortgageProperty(core::Property& prop) {
  core::Player* p = prop.getOwner();

  if (p != getCurrentPlayer()) {
    throw UnauthorizedActionException(getCurrentPlayer()->getName(),
                                      p ? p->getName() : "none",
                                      prop.getName());
  }
  if (prop.isMortgagedStatus()) {
    throw InvalidMoveException("Properti sudah digadaikan.");
  }

  prop.mortgage();
  bank_.pay(*p, prop.getMortgageValue());
  logEvent(data::LogAction::MORTGAGE, *p, prop, prop.getMortgageValue());
}

void Game::unmortgageProperty(core::Property& prop) {
  core::Player* p = prop.getOwner();

  if (p != getCurrentPlayer()) {
    throw UnauthorizedActionException(getCurrentPlayer()->getName(),
                                      p ? p->getName() : "none",
                                      prop.getName());
  }
  if (!prop.isMortgagedStatus()) {
    throw InvalidMoveException("Properti tidak sedang digadaikan.");
  }

  int cost = prop.getPrice();

  if (!p->canAfford(cost)) {
    throw InsufficientFundsException(p->getBalance(), cost);
  }

  *p -= cost;
  bank_.receive(cost);
  prop.unmortgage();
  logEvent(data::LogAction::UNMORTGAGE, *p, prop, cost);
}

void Game::giveCard(core::Player& player, core::ActionCard* card) {
  if (player.getHeldCards().size() >= 3) {
    core::ActionCard* dropped = mediator_->selectCardToDrop(player);
    if (dropped) {
      player.removeCard(dropped);
    }
  }
  player.addCard(card);
}

void Game::startAuction(core::Property& prop) {
  if (prop.getOwner() != nullptr) {
    throw InvalidMoveException("auction exception");
  }

  std::vector<core::Player*> eligiblePlayers;
  for (auto* p : players_) {
    if (!p->isBankrupted()) {
      eligiblePlayers.push_back(p);
    }
  }

  AuctionResult result = mediator_->runAuction(&prop, eligiblePlayers);
  if (result.winner != nullptr) {
    *result.winner -= result.finalBid;
    bank_.receive(result.finalBid);
    prop.setOwner(result.winner);
    result.winner->addProperty(prop);
    logEvent(data::LogAction::AUCTION_RESULT, *result.winner, prop,
             result.finalBid);
  }
}

void Game::logEvent(data::LogAction action, core::Player& p, int value) {
  data::TransactionLogger::get().log(turnCount_, p.getName(), action,
                                     "value=" + std::to_string(value));
}

void Game::logEvent(data::LogAction action, core::Player& p,
                    core::Property& prop, int value) {
  data::TransactionLogger::get().log(
      turnCount_, p.getName(), action,
      prop.getName() + " value=" + std::to_string(value));
}

core::Player* Game::getCurrentPlayer() const {
  if (players_.empty()) throw InvalidMoveException("log exception");
  return players_[currentPlayerId_];
}

GameState Game::getState() const { return state_; }

std::pair<int, int> Game::getLastDiceRoll() const { return lastDiceRoll_; }
int Game::getTurnCount() const { return turnCount_; }
int Game::getMaxTurn() const { return maxTurn_; }
int Game::getJailFine() const { return jailFine_; }

void Game::offerProperty(core::Player& p, core::Property& prop) {
  bool accept = mediator_->offerPropertyUI(p, prop);
  if (accept) {
    buyProperty(prop);
  } else {
    startAuction(prop);
  }
}

void Game::chargeRent(core::Player& p, core::Property& prop) {
  int rent = 0;
  int owned = 0;
  if (prop.getType() == core::PropertyTileType::RAILROAD ||
      prop.getType() == core::PropertyTileType::UTILITY) {
    owned = static_cast<int>(prop.getOwner()->getOwnedProperties().size());
    rent = prop.calculateRent(lastDiceRoll_.first + lastDiceRoll_.second, owned,
                              false);
  } else {
    rent = prop.calculateRent(lastDiceRoll_.first + lastDiceRoll_.second, 1,
                              false);
  }
  p -= rent;
  *(prop.getOwner()) += rent;
  logEvent(data::LogAction::RENT_PAYMENT, p, prop, rent);
}

void Game::sendToJail(core::Player& p) {
  p.goToJail();
  core::Tile* jailTile = board_.getTileByCode("PEN");
  if (jailTile) {
    p.setPosition(jailTile->getPosition());
  }
}

void Game::chargeTax(core::Player& p, int flatRate, int percentageRate,
                     core::TaxType type) {
  if (type == core::TaxType::PPH) {
    int percentageAmount = p.getBalance() * percentageRate / 100;
    bool usePercentage =
        mediator_->chooseTaxMethod(p, flatRate, percentageAmount);
    int amount = usePercentage ? percentageAmount : flatRate;
    p -= amount;
    bank_.receive(amount);
    logEvent(data::LogAction::TAX_PAYMENT, p, amount);
  } else {
    if (p.canAfford(flatRate)) {
      p -= flatRate;
      bank_.receive(flatRate);
      logEvent(data::LogAction::TAX_PAYMENT, p, flatRate);
    } else {
      p.declareBankrupt();
      logEvent(data::LogAction::BANKRUPTCY, p, flatRate);
    }
  }
}

void Game::activateFestival(core::Player& p) {
  if (p.getOwnedProperties().empty()) {
    throw InvalidMoveException("festival exception");
  }
  core::Property* selectedProp = mediator_->selectFestivalProperty(p);
  if (selectedProp != nullptr) {
    resolveFestival(*selectedProp);
  }
}

void Game::resolveFestival(core::Property& selectedProp) {
  selectedProp.applyFestival();
  logEvent(data::LogAction::FESTIVAL_ACTIVATION, *getCurrentPlayer(),
           selectedProp, selectedProp.getFestMultiplier());
}

void Game::drawChanceCard(core::Player& p) {
  logEvent(data::LogAction::CHANCE_CARD_DRAW, p, 0);
  core::ActionCard* card = chanceDeck_.draw();
  if (card) {
    card->execute(p, *this);
  }
}

void Game::drawCommunityChestCard(core::Player& p) {
  logEvent(data::LogAction::COMMUNITY_CARD_DRAW, p, 0);
  core::ActionCard* card = communityChestDeck_.draw();
  if (card) {
    card->execute(p, *this);
  }
}

void Game::payPlayerFromBank(core::Player& p, int amount) {
  bank_.pay(p, amount);
}

int Game::getGoSalary() const { return goSalary_; }

core::Player* Game::getPlayerByName(const std::string& name) const {
  for (auto* p : players_)
    if (p && p->getName() == name) return p;
  return nullptr;
}

void Game::setTurnCount(int count) { turnCount_ = count; }
void Game::setMaxTurn(int max) { maxTurn_ = max; }
void Game::setCurrentPlayerIdx(int idx) { currentPlayerId_ = idx; }

void Game::setTurnOrder(const std::vector<std::string>& order) {
  std::vector<core::Player*> reordered;
  reordered.reserve(order.size());
  for (const auto& name : order) {
    for (auto* p : players_) {
      if (p && p->getName() == name) {
        reordered.push_back(p);
        break;
      }
    }
  }
  if (reordered.size() == players_.size()) players_ = reordered;
}

core::CardDeck<core::ActionCard>& Game::getSkillDeck() { return skillDeck_; }

void Game::buildChanceDeck() {
  std::vector<std::unique_ptr<core::ActionCard>> cards;
  cards.push_back(core::ChanceCard::makeAdvanceToNearestRailroad(
      "Pergi ke stasiun terdekat."));
  cards.push_back(core::ChanceCard::makeMoveBack(3, "Mundur 3 petak."));
  cards.push_back(core::ChanceCard::makeGoToJail("Masuk Penjara."));
  chanceDeck_ = core::CardDeck<core::ActionCard>(std::move(cards));
  chanceDeck_.shuffle();
}

void Game::buildCommunityChestDeck() {
  std::vector<std::unique_ptr<core::ActionCard>> cards;
  cards.push_back(core::CommunityChestCard::makeCollectFromAll(
      100, "Ulang tahun: dapat M100 dari tiap pemain."));
  cards.push_back(
      core::CommunityChestCard::makePayBank(700, "Biaya dokter: bayar M700."));
  cards.push_back(core::CommunityChestCard::makePayToAll(
      200, "Nyaleg: bayar M200 ke tiap pemain."));
  communityChestDeck_ = core::CardDeck<core::ActionCard>(std::move(cards));
  communityChestDeck_.shuffle();
}

void Game::buildSkillDeck() {
  std::vector<std::unique_ptr<core::ActionCard>> cards;
  std::uniform_int_distribution<int> stepDist(1, 6);

  for (int i = 0; i < 4; ++i) {
    const int steps = stepDist(rng_);
    cards.push_back(core::MoveCard::make(
        steps, std::string("Maju ") + std::to_string(steps) + " petak."));
  }
  for (int i = 0; i < 3; ++i) {
    cards.push_back(core::DiscountCard::makeRandom("Diskon properti."));
  }
  for (int i = 0; i < 2; ++i) {
    cards.push_back(core::ShieldCard::make("Tameng pajak."));
  }
  for (int i = 0; i < 2; ++i) {
    cards.push_back(core::TeleportCard::make(0, "Teleportasi ke GO."));
  }
  for (int i = 0; i < 2; ++i) {
    cards.push_back(core::LassoCard::make("Laso pemain terdekat."));
  }
  for (int i = 0; i < 2; ++i) {
    cards.push_back(core::DemolitionCard::make("Demolisi bangunan lawan."));
  }

  skillDeck_ = core::CardDeck<core::ActionCard>(std::move(cards));
  skillDeck_.shuffle();
}

const std::vector<core::Player*>& Game::getPlayers() const { return players_; }

int Game::getBoardSize() const { return board_.getTileCount(); }

void Game::movePlayer(core::Player& p, int targetIndex) {
  int boardSize = board_.getTileCount();
  if (boardSize <= 0) return;
  int dest = ((targetIndex % boardSize) + boardSize) % boardSize;
  stepThrough(p, p.getPosition(), dest, /*firePassed=*/true);
}

void Game::teleportPlayer(core::Player& p, int targetIndex) {
  int boardSize = board_.getTileCount();
  if (boardSize <= 0) return;
  int dest = ((targetIndex % boardSize) + boardSize) % boardSize;
  stepThrough(p, p.getPosition(), dest, /*firePassed=*/false);
}

int Game::findNearestTileOfType(int from, core::TileType type) const {
  int boardSize = board_.getTileCount();
  if (boardSize <= 0) return -1;
  int origin = ((from % boardSize) + boardSize) % boardSize;
  for (int step = 1; step <= boardSize; ++step) {
    int idx = (origin + step) % boardSize;
    const core::Tile* tile = board_.getTile(idx);
    if (tile && tile->getType() == type) {
      return idx;
    }
  }
  return -1;
}

int Game::findNearestPropertyTileType(int from,
                                      core::PropertyTileType type) const {
  int boardSize = board_.getTileCount();
  if (boardSize <= 0) return -1;
  int origin = ((from % boardSize) + boardSize) % boardSize;
  for (int step = 1; step <= boardSize; ++step) {
    int idx = (origin + step) % boardSize;
    const core::Tile* tile = board_.getTile(idx);
    if (tile && tile->getType() == core::TileType::PROPERTY) {
      const auto* propertyTile = static_cast<const core::PropertyTile*>(tile);
      if (propertyTile->getPropertyTileType() == type) {
        return idx;
      }
    }
  }
  return -1;
}

}  // namespace logic
