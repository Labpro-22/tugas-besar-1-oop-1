#include "logic/BankruptcyHandler.hpp"

#include "core/Player.hpp"

namespace logic {

void BankruptcyHandler::handle(core::Player& debtor, core::Player& creditor) {
  (void)debtor;
  (void)creditor;
}

int BankruptcyHandler::calcLiquidationValue(core::Player& player) {
  (void)player;
  return 0;
}

void BankruptcyHandler::transferAssets(core::Player& from, core::Player& to) {
  (void)from;
  (void)to;
}

}  // namespace logic
