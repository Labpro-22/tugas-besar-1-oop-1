#include "../../include/core/Property.hpp"
#include "../../include/core/Player.hpp"
#include "../../include/logic/Game.hpp"

using namespace std;

namespace core {

Utility::Utility(string name, int mortgageValue, map<int, int> multiplierTable) : Property(move(name), 0, mortgageValue), multiplierTable_(move(multiplierTable)) {}

int Utility::calculateRent(logic::Game& g) {
    if (isMortgagedStatus()) {
        return 0;
    }

    Player* owner = getOwner();
    if (!owner) {
        return 0;
    }

    auto properties = g.getPropertiesOwnedBy(owner);
    int ownedUtilities = 0;
    for (auto* prop : properties) {
        if (prop->getType() == "UTILITY" && !prop->isMortgagedStatus()) {
            ownedUtilities++;
        }
    }

    int multiplier = 1;
    auto it = multiplierTable_.find(ownedUtilities);
    if (it != multiplierTable_.end()) {
        multiplier = it->second;
    } else if (!multiplierTable_.empty()) {
        multiplier = multiplierTable_.rbegin()->second;
    }

    return g.getLastDiceRoll() * multiplier;
}

string Utility::getType() const { 
    return "UTILITY"; 
}

int Utility::getCurrentMultiplier(logic::Game& g) const {
    Player* owner = getOwner();
    if (!owner) {
        return 1;
    }

    auto properties = g.getPropertiesOwnedBy(owner);
    int ownedUtilities = 0;
    for (auto* prop : properties) {
        if (prop->getType() == "UTILITY" && !prop->isMortgagedStatus()) {
            ownedUtilities++;
        }
    }

    auto it = multiplierTable_.find(ownedUtilities);
    if (it != multiplierTable_.end()) {
        return it->second;
    }
    if (!multiplierTable_.empty()) {
        return multiplierTable_.rbegin()->second;
    }

    return 1;
}

}