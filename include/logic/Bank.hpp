#pragma once

namespace core {
class Player;
}

namespace logic {

class Bank {
private:
  int totalMoney_;

public:
  Bank(int initialMoney = 999999);
  ~Bank() = default;
  void pay(core::Player &p, int amount);
  void receive(int amount);
};

} // namespace logic
