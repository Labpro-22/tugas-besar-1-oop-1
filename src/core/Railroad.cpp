#include <cmath>

#include "core/Property.hpp"

using namespace std;

namespace core {

map<int, int> Railroad::rentTable_;

Railroad::Railroad(const string& name, int mortgageValue, int baseFare)
    : Property(name, 0, mortgageValue), baseFare_(baseFare) {}

void Railroad::setRentTable(const map<int, int>& table) { rentTable_ = table; }

int Railroad::calculateRent([[maybe_unused]] int diceRoll, int ownedSameType,
                            [[maybe_unused]] bool hasMonopoly) const {
  if (isMortgagedStatus() || !isOwned()) return 0;
  if (ownedSameType <= 0) return 0;

  int rent = 0;
  if (!rentTable_.empty()) {
    // Gunakan tabel dari config (lebih akurat dan config-driven)
    auto it = rentTable_.find(ownedSameType);
    if (it != rentTable_.end()) {
      rent = it->second;
    } else {
      // Fallback ke entry tertinggi jika count melebihi tabel
      rent = rentTable_.rbegin()->second;
    }
  } else {
    // Fallback ke formula jika setRentTable belum dipanggil
    rent = static_cast<int>(baseFare_ * pow(2, ownedSameType - 1));
  }

  return rent * getFestMultiplier();
}

PropertyType Railroad::getType() const { return PropertyType::RAILROAD; }

int Railroad::getBaseFare() const { return baseFare_; }

}  // namespace core