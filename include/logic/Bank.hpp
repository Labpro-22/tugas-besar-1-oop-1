#pragma once

namespace core {
class Player;
class Property;
class Street;
}

namespace logic {

class Bank {
 public:
  Bank() = default;
  ~Bank() = default;

  void pay(core::Player& p, int amount);
  void receive(int amount);

  bool processPayment(core::Player& payer, int amount,
                      core::Player* creditor = nullptr);
  int buyProperty(core::Player& buyer, core::Property& prop);
  int buildHouse(core::Player& buyer, core::Street& street);
  int sellHouse(core::Player& seller, core::Street& street);
  int mortgageProperty(core::Player& actor, core::Property& prop);
  int unmortgageProperty(core::Player& actor, core::Property& prop);
  int collectRent(core::Player& payer, core::Property& prop, int diceRoll);
  int collectTax(core::Player& payer, int amount);

  void handleBankruptcy(core::Player& debtor, core::Player* creditor);
  int calcLiquidationValue(core::Player& player);
  void transferAssets(core::Player& from, core::Player& to);
  void repossessAssets(core::Player& debtor);
};

}  // namespace logic
