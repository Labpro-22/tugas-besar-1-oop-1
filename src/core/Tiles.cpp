#include "core/Tiles.hpp"

#include "core/GameContext.hpp"
#include "data/LogEntry.hpp"

namespace core {

Tile::Tile(int position, const std::string& name)
    : position_(position), name_(name) {}

void Tile::onPassed(Player&, GameContext&) {
  // do nothing, default
}

std::string Tile::getName() const { return this->name_; }

int Tile::getPosition() const { return position_; }

ActionTile::ActionTile(int position, const std::string& name,
                       ActionTileType actionType)
    : Tile(position, name), actionType_(actionType) {}

TileType ActionTile::getType() const { return TileType::ACTION; }

ActionTileType ActionTile::getActionTileType() const { return actionType_; }

SpecialTile::SpecialTile(int position, const std::string& name,
                         SpecialTileType specialType)
    : ActionTile(position, name, ActionTileType::SPECIAL),
      specialType_(specialType) {}

SpecialTileType SpecialTile::getSpecialTileType() const { return specialType_; }

PropertyTile::PropertyTile(int position, const std::string& name,
                           PropertyTileType propertyType)
    : Tile(position, name), propertyType_(propertyType) {}

TileType PropertyTile::getType() const { return TileType::PROPERTY; }

PropertyTileType PropertyTile::getPropertyTileType() const {
  return propertyType_;
}

void PropertyTile::onLanded(Player& p, GameContext& g) {
  Property& property = getProperty();

  if (!property.isOwned()) {
    // purchase
    g.offerProperty(p, property);
  } else if (property.getOwner() != &p && !property.isMortgagedStatus()) {
    // rent
    g.chargeRent(p, property);
  }
}

StreetTile::StreetTile(int position, const std::string& name, Street property)
    : PropertyTile(position, name, PropertyTileType::STREET),
      property_(std::move(property)) {}

Property& StreetTile::getProperty() { return property_; }

const Property& StreetTile::getProperty() const { return property_; }

UtilityTile::UtilityTile(int position, const std::string& name,
                         Utility property)
    : PropertyTile(position, name, PropertyTileType::UTILITY),
      property_(std::move(property)) {}

Property& UtilityTile::getProperty() { return property_; }

const Property& UtilityTile::getProperty() const { return property_; }

void UtilityTile::onLanded(Player& p, GameContext& g) {
  Property& property = getProperty();
  if (!property.isOwned()) {
    property.setOwner(&p);
    p.addProperty(property);
    g.logEvent(data::LogAction::PROPERTY_PURCHASE, p, property, 0);
  } else if (property.getOwner() != &p && !property.isMortgagedStatus()) {
    g.chargeRent(p, property);
  }
}

RailroadTile::RailroadTile(int position, const std::string& name,
                           Railroad property)
    : PropertyTile(position, name, PropertyTileType::RAILROAD),
      property_(std::move(property)) {}

Property& RailroadTile::getProperty() { return property_; }

const Property& RailroadTile::getProperty() const { return property_; }

void RailroadTile::onLanded(Player& p, GameContext& g) {
  Property& property = getProperty();
  if (!property.isOwned()) {
    g.offerProperty(p, property);
  } else if (property.getOwner() != &p && !property.isMortgagedStatus()) {
    g.chargeRent(p, property);
  }
}

GoTile::GoTile(int position, const std::string& name)
    : SpecialTile(position, name, SpecialTileType::GO) {}

void GoTile::onLanded(Player& p, GameContext& g) {
  // get salary added to their inventoryt
  g.payPlayerFromBank(p, g.getGoSalary());
  g.logEvent(data::LogAction::PIECE_MOVEMENT, p, g.getGoSalary());
}

void GoTile::onPassed(Player& p, GameContext& g) {
  // get salary again
  g.payPlayerFromBank(p, g.getGoSalary());
  g.logEvent(data::LogAction::PIECE_MOVEMENT, p, g.getGoSalary());
}

FreeParkingTile::FreeParkingTile(int position, const std::string& name)
    : SpecialTile(position, name, SpecialTileType::FREE_PARKING) {}

void FreeParkingTile::onLanded(Player&, GameContext&) {
  // nothing happens here
}

JailTile::JailTile(int position, const std::string& name)
    : SpecialTile(position, name, SpecialTileType::JAIL) {}
void JailTile::onLanded(Player&, GameContext&) {
  // just visiting
}

GoToJailTile::GoToJailTile(int position, const std::string& name)
    : SpecialTile(position, name, SpecialTileType::GO_TO_JAIL) {}

void GoToJailTile::onLanded(Player& p, GameContext& g) {
  g.sendToJail(p);
  g.logEvent(data::LogAction::PIECE_MOVEMENT, p, 0);
}

TaxTile::TaxTile(int position, const std::string& name, int flatRate,
                 int percentageRate, TaxType type)
    : ActionTile(position, name, ActionTileType::TAX),
      flatRate_(flatRate),
      percentageRate_(percentageRate),
      type_(type) {}

void TaxTile::onLanded(Player& p, GameContext& g) {
  g.chargeTax(p, flatRate_, percentageRate_, type_);
}

int TaxTile::getFlatRate() const { return flatRate_; }
int TaxTile::getPercentageRate() const { return percentageRate_; }
TaxType TaxTile::getTaxType() const { return type_; }

FestivalTile::FestivalTile(int position, const std::string& name)
    : ActionTile(position, name, ActionTileType::FESTIVAL) {}

void FestivalTile::onLanded([[maybe_unused]] Player& p,
                            [[maybe_unused]] GameContext& g) {
  // pick property
  g.activateFestival(p);
}

CardTile::CardTile(int position, const std::string& name, bool isChance)
    : ActionTile(position, name, ActionTileType::CARD), isChance_(isChance) {}

void CardTile::onLanded(Player& p, GameContext& g) {
  if (isChance_) {
    g.drawChanceCard(p);
  } else {
    g.drawCommunityChestCard(p);
  }
}

bool CardTile::isChance() const { return isChance_; }

}  // namespace core
