#include "core/player/COMPlayer.hpp"

#include <utility>

#include "core/Property.hpp"

namespace core {

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

void COMPlayer::takeTurn(PlayerTurnContext& turnContext) {
  resetPerTurnFlags();
  turnContext.rollDice();
  turnContext.moveCurrentPlayer();

  if (strategy_ == nullptr) {
    return;
  }

  for (Property* lot : getOwnedProperties()) {
    if (lot != nullptr && strategy_->shouldMortgage(lot, *this)) {
      turnContext.mortgageProperty(*this, *lot);
    }
  }

  for (Property* lot : getOwnedProperties()) {
    if (lot == nullptr || !strategy_->shouldBuild(lot, *this)) {
      continue;
    }
    turnContext.buildHouse(*this, *lot);
    break;
  }
}

bool COMPlayer::isHuman() { return false; }

}  // namespace core
