#include "logic/BankruptcyHandler.hpp"

#include "core/GameException.hpp"
#include "core/Property.hpp"

namespace logic {

void BankruptcyHandler::handle(core::Player& debtor, core::Player* creditor) {
  if (creditor != nullptr) {
    transferAssets(debtor, *creditor);
  }
  debtor.setBankrupted(true);
}

int BankruptcyHandler::calcLiquidationValue(core::Player& player) {
  int total = player.getBalance();
  for (core::Property* prop : player.getOwnedProperties()) {
    if (prop != nullptr) {
      total += prop->isMortgagedStatus() ? prop->getMortgageValue()
                                         : prop->getPrice();
    }
  }
  return total;
}

void BankruptcyHandler::transferAssets(core::Player& from, core::Player& to) {
  int cash = from.getBalance();
  if (cash > 0) {
    to += cash;
    from -= cash;
  }

  auto properties = from.getOwnedProperties();
  for (core::Property* prop : properties) {
    if (prop == nullptr) continue;
    prop->setOwner(&to);
    to.addProperty(*prop);
    from.removeProperty(*prop);
  }

  auto cards = from.getHeldCards();
  for (core::ActionCard* card : cards) {
    to.addCard(card);
    from.removeCard(card);
  }
}

}  // namespace logic
