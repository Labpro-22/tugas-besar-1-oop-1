#include "core/player/COMPlayer.hpp"

#include <utility>

#include "core/Property.hpp"
#include "core/Tiles.hpp"
#include "logic/Board.hpp"
#include "logic/Game.hpp"

namespace core {

namespace {

core::Tile* findTileFor(logic::Board& board, Property& target) {
  for (int i = 0; i < board.getTileCount(); ++i) {
    core::Tile* t = board.getTile(i);
    if (t != nullptr && t->getType() == core::TileType::PROPERTY) {
      auto* propertyTile = static_cast<core::PropertyTile*>(t);
      if (&propertyTile->getProperty() == &target) {
        return t;
      }
    }
  }
  return nullptr;
}

}  // namespace

COMPlayer::COMPlayer(std::string name, Avatar avatar,
                     std::unique_ptr<COMStrategy> strategy)
    : Player(std::move(name), avatar),
      strategy_(std::move(strategy)) {}

void COMPlayer::setStrategy(std::unique_ptr<COMStrategy> strategy) {
  strategy_ = std::move(strategy);
}

COMStrategy* COMPlayer::getStrategy() const noexcept { return strategy_.get(); }

int COMPlayer::promptChoice(const std::string& context, int defaultIndex,
                            int optionCount) {
  if (strategy_ != nullptr) {
    return strategy_->suggestChoice(context, defaultIndex, optionCount);
  }
  return Player::promptChoice(context, defaultIndex, optionCount);
}

void COMPlayer::takeTurn(logic::Game& game) {
  resetPerTurnFlags();
  game.rollDice();
  game.moveCurrentPlayer();

  if (strategy_ == nullptr) {
    return;
  }

  for (Property* lot : getOwnedProperties()) {
    if (lot != nullptr && strategy_->shouldMortgage(lot, *this)) {
      game.mortgageProperty(*lot);
    }
  }

  for (Property* lot : getOwnedProperties()) {
    if (lot == nullptr || !strategy_->shouldBuild(lot, *this)) {
      continue;
    }
    core::Tile* at = findTileFor(game.getBoard(), *lot);
    if (at != nullptr) {
      game.buildHouse(this, at);
      break;
    }
  }
}

bool COMPlayer::isHuman() { return false; }

}  // namespace core
