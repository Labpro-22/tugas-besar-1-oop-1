#include "core/player/SimpleCOMStrategy.hpp"

#include "core/Property.hpp"
#include "core/Tiles.hpp"
#include "core/player/Player.hpp"
#include "logic/Game.hpp"

namespace core {

bool SimpleCOMStrategy::shouldBuy(Property* property, Player& player,
                                  logic::Game& game) const noexcept {
  (void)game;
  if (property == nullptr) {
    return false;
  }
  const int price = property->getPrice();
  return price <= static_cast<int>(player.getBalance() * 0.4);
}

int SimpleCOMStrategy::bidAmount(Property* property, Player& player,
                                 logic::Game& game,
                                 int previous_bid) const noexcept {
  (void)game;
  if (property == nullptr) {
    return previous_bid;
  }
  const int cap =
      static_cast<int>(static_cast<double>(property->getPrice()) * 1.1);
  const int balance = player.getBalance();
  if (cap > balance) {
    return balance;
  }
  if (cap < previous_bid) {
    return previous_bid;
  }
  return cap;
}

bool SimpleCOMStrategy::shouldBuild(Property* property, Player& player,
                                    logic::Game& game) const noexcept {
  (void)game;
  if (property == nullptr || property->getType() != PropertyTileType::STREET) {
    return false;
  }
  const int houseCost = static_cast<Street*>(property)->getHouseCost();
  if (houseCost <= 0) {
    return false;
  }
  return player.getBalance() > 3 * houseCost;
}

bool SimpleCOMStrategy::shouldMortgage(Property* property, Player& player,
                                       logic::Game& game) const noexcept {
  (void)property;
  (void)game;
  return player.getBalance() < 200;
}

}  // namespace core
