#include "logic/Bank.hpp"

#include "core/GameException.hpp"
#include "core/player/Player.hpp"
namespace logic {

Bank::Bank(int initialMoney) : totalMoney_(initialMoney) {}

void Bank::pay(core::Player &p, int amount) {
  totalMoney_ -= amount;  // decrease bank money
  p += amount;            // player money increase
}

void Bank::receive(int amount) { totalMoney_ += amount; }

}  // namespace logic
