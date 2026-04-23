#pragma once

namespace core {

class Player;

}  // namespace core

namespace logic {

class BankruptcyHandler {
 public:
  void handle(core::Player& debtor, core::Player& creditor);
  int calcLiquidationValue(core::Player& player);
  void transferAssets(core::Player& from, core::Player& to);
};

}  // namespace logic
