#include <algorithm>

#include "core/GameException.hpp"
#include "core/Property.hpp"
#include "core/Tiles.hpp"

namespace core {

Street::Street(const std::string& name, int price, int mortgageValue,
               ColorGroup colorGroup, std::vector<int> rentValues,
               int houseCost, int hotelCost)
    : Property(name, price, mortgageValue),
      colorGroup_(colorGroup),
      houseCount_(0),
      hotelCount_(0),
      rentValues_(std::move(rentValues)),
      houseCost_(houseCost),
      hotelCost_(hotelCost) {
  if (rentValues_.size() != 6)
    throw InvalidConfigException("rentValues untuk '" + getName() + "'",
                                 "tepat 6 elemen (level 0-5)");
}

int Street::calculateRent([[maybe_unused]] int diceRoll,
                          [[maybe_unused]] int ownedSameType,
                          bool hasMonopoly) const {
  if (isMortgagedStatus()) return 0;

  int rent;
  if (hotelCount_ > 0) {
    rent = rentValues_[5];
  } else if (houseCount_ > 0) {
    rent = rentValues_[houseCount_];
  } else if (hasMonopoly) {
    rent = rentValues_[0] * 2;
  } else {
    rent = rentValues_[0];
  }

  return rent * getFestMultiplier();
}

PropertyTileType Street::getType() const { return PropertyTileType::STREET; }

void Street::build() {
  if (hotelCount_ > 0)
    throw InvalidMoveException("Petak '" + getName() +
                               "' sudah memiliki hotel");

  if (houseCount_ < 4) {
    ++houseCount_;
  } else {
    houseCount_ = 0;
    hotelCount_ = 1;
  }
}

void Street::demolish(int n) {
  if (n <= 0) return;
  if (hotelCount_ > 0) {
    hotelCount_ = 0;
    --n;
  }
  int toRemove = std::min(n, houseCount_);
  houseCount_ -= toRemove;
}

void Street::setLevel(int level) {
  if (level < 0 || level > 5)
    throw InvalidConfigException("Street level", "0-4 (rumah) atau 5 (hotel)");
  if (level == 5) {
    houseCount_ = 0;
    hotelCount_ = 1;
  } else {
    houseCount_ = level;
    hotelCount_ = 0;
  }
}

bool Street::canBuild(bool hasMonopoly) const {
  return hasMonopoly && hotelCount_ == 0;
}

ColorGroup Street::getColorGroup() const { return colorGroup_; }
int Street::getHouseCount() const { return houseCount_; }
int Street::getHotelCount() const { return hotelCount_; }
int Street::getHouseCost() const { return houseCost_; }
int Street::getHotelCost() const { return hotelCost_; }

}  // namespace core