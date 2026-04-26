#include "core/player/GreedyCOMStrategy.hpp"

#include <algorithm>

#include "core/Property.hpp"
#include "core/Tiles.hpp"
#include "core/player/Player.hpp"

namespace core {

bool GreedyCOMStrategy::shouldBuy(Property* property,
                                  Player& player) const noexcept {
  if (property == nullptr) {
    return false;
  }
  return player.canAfford(property->getPrice());
}

int GreedyCOMStrategy::bidAmount(Property* property, Player& player,
                                 int previous_bid) const noexcept {
  (void)property;
  const int aggressive =
      static_cast<int>(static_cast<double>(previous_bid) * 1.25 + 50.0);
  return std::min(player.getBalance(), aggressive);
}

bool GreedyCOMStrategy::shouldBuild(Property* property,
                                    Player& player) const noexcept {
  if (property == nullptr || property->getType() != PropertyTileType::STREET) {
    return false;
  }
  const int houseCost = static_cast<Street*>(property)->getHouseCost();
  if (houseCost <= 0) {
    return false;
  }
  return player.canAfford(houseCost);
}

bool GreedyCOMStrategy::shouldMortgage(Property* property,
                                       Player& player) const noexcept {
  (void)property;
  return player.getBalance() <= 0;
}

int GreedyCOMStrategy::suggestChoice(const std::string& context,
                                     int defaultIndex,
                                     int optionCount) const noexcept {
  (void)context;
  if (optionCount > 0) {
    return optionCount - 1;
  }
  return defaultIndex;
}

}  // namespace core
