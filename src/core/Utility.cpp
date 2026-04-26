#include "core/Property.hpp"

using namespace std;

namespace core {
map<int, int> Utility::multiplierTable_;

Utility::Utility(const string& name, int mortgageValue)
    : Property(name, 0, mortgageValue) {}

void Utility::setMultiplierTabel(const map<int, int>& table) {
  multiplierTable_ = table;
}

int Utility::calculateRent(int diceRoll, int ownedSameType,
                           [[maybe_unused]] bool hasMonopoly) const {
  if (isMortgagedStatus() || !isOwned()) {
    return 0;
  }

  int multiplier = getCurrentMultiplier(ownedSameType);
  int rent = diceRoll * multiplier;
  return rent * getFestMultiplier();
}

PropertyType Utility::getType() const { return PropertyType::UTILITY; }

int Utility::getCurrentMultiplier(int ownedUtilities) const {
  auto it = multiplierTable_.find(ownedUtilities);
  if (it != multiplierTable_.end()) {
    return it->second;
  }
  if (!multiplierTable_.empty()) {
    return multiplierTable_.rbegin()->second;
  }

  return 1;
}

}  // namespace core
