#include "logic/Bank.hpp"
#include "core/GameException.hpp"
#include "core/Player.hpp"
namespace logic {

Bank::Bank(int initialMoney, int initialHouses, int initialHotels)
    : totalMoney_(initialMoney), housesAvailable_(initialHouses),
      hotelsAvailable_(initialHotels) {}

bool Bank::canBuyHouse(int count) const { return housesAvailable_ >= count; }

bool Bank::canBuyHotel(int count) const { return hotelsAvailable_ >= count; }

void Bank::sellHouse(int count) {
  if (!canBuyHouse(count)) {
    throw InvalidMoveException("Bank tidak memiliki cukup rumah");
  }
  housesAvailable_ -= count;
}

void Bank::sellHotel(int count) {
  if (!canBuyHotel(count)) {
    throw InvalidMoveException("Bank tidak memmiliki cukup hotel");
  }

  hotelsAvailable_ -= count;
}

void Bank::receiveHouse(int count) { housesAvailable_ += count; }

void Bank::receiveHotel(int count) { hotelsAvailable_ += count; }

void Bank::pay(core::Player &p, int amount) {
  totalMoney_ -= amount; // decrease bank money
  p + amount;            // player money increase
}

void Bank::receive(int amount) { totalMoney_ += amount; }

} // namespace logic
