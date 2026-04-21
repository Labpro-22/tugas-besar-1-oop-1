#include "include/core/Property.hpp"

#include <cmath>

using namespace std;

namespace core {

Railroad::Railroad(const string& name, int mortgageValue, int baseFare) : Property(name, 0, mortgageValue), baseFare_(baseFare) {}

int Railroad::calculateRent([[maybe_unused]] int diceRoll, int ownedSameType, [[maybe_unused]] bool haMonopoly) const {
    if (isMortgagedStatus() || !isOwned()) {
        return 0;
    } 

    if (ownedSameType <= 0) {
        return 0;
    }

    int rent = static_cast<int>(baseFare_ * pow(2, ownedSameType - 1));
    return rent * getFestMultiplier();
}

PropertyType Railroad::getType() const { 
    return PropertyType::RAILROAD; 
}

int Railroad::getBaseFare() const { 
    return baseFare_; 
}

}