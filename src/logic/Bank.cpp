#include "logic/Bank.hpp"

#include "core/GameException.hpp"
#include "core/Property.hpp"
#include "core/Tiles.hpp"
#include "core/card/Card.hpp"
#include "core/player/Player.hpp"

namespace logic {

namespace {

int countOwnedPropertiesOfType(const core::Player& owner,
                               core::PropertyTileType type) {
  int count = 0;
  for (core::Property* property : owner.getOwnedProperties()) {
    if (property != nullptr && property->getType() == type) {
      ++count;
    }
  }
  return count;
}

}  // namespace

void Bank::pay(core::Player& p, int amount) { p += amount; }

void Bank::receive(int /*amount*/) {}

bool Bank::processPayment(core::Player& payer, int amount,
                          core::Player* creditor) {
  if (amount <= 0) {
    return true;
  }
  if (!payer.canAfford(amount)) {
    handleBankruptcy(payer, creditor);
    return false;
  }

  payer -= amount;
  if (creditor != nullptr) {
    *creditor += amount;
  } else {
    receive(amount);
  }
  return true;
}

int Bank::buyProperty(core::Player& buyer, core::Property& prop) {
  if (prop.getOwner() != nullptr) {
    throw InvalidMoveException("owned property exception");
  }

  const int price = static_cast<int>(
      static_cast<float>(prop.getPrice()) * (1.0F - buyer.getDiscountRate()));
  if (!buyer.canAfford(price)) {
    throw InsufficientFundsException(buyer.getBalance(), price);
  }

  buyer -= price;
  receive(price);
  prop.setOwner(&buyer);
  buyer.addProperty(prop);
  return price;
}

int Bank::awardAuction(core::Player& winner, core::Property& prop, int amount) {
  if (prop.getOwner() != nullptr) {
    throw InvalidMoveException("owned property exception");
  }
  if (amount <= 0) {
    throw InvalidMoveException("auction bid must be positive");
  }
  if (!winner.canAfford(amount)) {
    throw InsufficientFundsException(winner.getBalance(), amount);
  }

  winner -= amount;
  receive(amount);
  prop.setOwner(&winner);
  winner.addProperty(prop);
  return amount;
}

int Bank::buildHouse(core::Player& buyer, core::Street& street) {
  if (street.getOwner() != &buyer) {
    throw UnauthorizedActionException(
        buyer.getName(),
        street.getOwner() ? street.getOwner()->getName() : "none",
        street.getName());
  }

  const int cost = street.getHouseCost();
  if (!buyer.canAfford(cost)) {
    throw InsufficientFundsException(buyer.getBalance(), cost);
  }

  buyer -= cost;
  receive(cost);
  street.build();
  return cost;
}

int Bank::sellHouse(core::Player& seller, core::Street& street) {
  if (street.getHouseCount() == 0 && street.getHotelCount() == 0) {
    throw InvalidMoveException("no building exception");
  }

  if (street.getOwner() != &seller) {
    throw UnauthorizedActionException(
        seller.getName(),
        street.getOwner() ? street.getOwner()->getName() : "none",
        street.getName());
  }

  const int refund = street.getHouseCost() / 2;
  street.demolish(1);
  pay(seller, refund);
  return refund;
}

int Bank::mortgageProperty(core::Player& actor, core::Property& prop) {
  core::Player* owner = prop.getOwner();
  if (owner != &actor) {
    throw UnauthorizedActionException(actor.getName(),
                                      owner ? owner->getName() : "none",
                                      prop.getName());
  }
  if (prop.isMortgagedStatus()) {
    throw InvalidMoveException("Properti sudah digadaikan.");
  }

  prop.mortgage();
  pay(actor, prop.getMortgageValue());
  return prop.getMortgageValue();
}

int Bank::unmortgageProperty(core::Player& actor, core::Property& prop) {
  core::Player* owner = prop.getOwner();
  if (owner != &actor) {
    throw UnauthorizedActionException(actor.getName(),
                                      owner ? owner->getName() : "none",
                                      prop.getName());
  }
  if (!prop.isMortgagedStatus()) {
    throw InvalidMoveException("Properti tidak sedang digadaikan.");
  }

  const int cost = prop.getMortgageValue() + prop.getMortgageValue() / 10;
  if (!actor.canAfford(cost)) {
    throw InsufficientFundsException(actor.getBalance(), cost);
  }

  actor -= cost;
  receive(cost);
  prop.unmortgage();
  return cost;
}

int Bank::collectRent(core::Player& payer, core::Property& prop, int diceRoll) {
  core::Player* owner = prop.getOwner();
  if (owner == nullptr) {
    throw InvalidMoveException("rent owner exception");
  }

  int owned = 1;
  if (prop.getType() == core::PropertyTileType::RAILROAD) {
    owned = countOwnedPropertiesOfType(*owner, core::PropertyTileType::RAILROAD);
  } else if (prop.getType() == core::PropertyTileType::UTILITY) {
    owned = countOwnedPropertiesOfType(*owner, core::PropertyTileType::UTILITY);
  }

  const int rent = prop.calculateRent(diceRoll, owned, false);
  processPayment(payer, rent, owner);
  return rent;
}

int Bank::collectTax(core::Player& payer, int amount) {
  processPayment(payer, amount, nullptr);
  return amount;
}

void Bank::handleBankruptcy(core::Player& debtor, core::Player* creditor) {
  if (creditor != nullptr) {
    transferAssets(debtor, *creditor);
  } else {
    repossessAssets(debtor);
  }
  debtor.declareBankrupt();
}

int Bank::calcLiquidationValue(core::Player& player) {
  int total = player.getBalance();
  for (core::Property* prop : player.getOwnedProperties()) {
    if (prop != nullptr) {
      total += prop->isMortgagedStatus() ? prop->getMortgageValue()
                                         : prop->getPrice();
    }
  }
  return total;
}

void Bank::transferAssets(core::Player& from, core::Player& to) {
  const int cash = from.getBalance();
  if (cash > 0) {
    to += cash;
    from -= cash;
  }

  auto properties = from.getOwnedProperties();
  for (core::Property* prop : properties) {
    if (prop == nullptr) {
      continue;
    }
    prop->setOwner(&to);
    to.addProperty(*prop);
    from.removeProperty(*prop);
  }

  auto cards = from.getHeldCards();
  for (core::ActionCard* card : cards) {
    to.addCard(from.removeCard(card));
  }
}

void Bank::repossessAssets(core::Player& debtor) {
  const int cash = debtor.getBalance();
  if (cash > 0) {
    debtor -= cash;
  }

  auto properties = debtor.getOwnedProperties();
  for (core::Property* prop : properties) {
    if (prop == nullptr) {
      continue;
    }
    prop->setOwner(nullptr);
    debtor.removeProperty(*prop);
  }

  auto cards = debtor.getHeldCards();
  for (core::ActionCard* card : cards) {
    debtor.removeCard(card);
  }
}

}  // namespace logic
