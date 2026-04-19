#include "../../include/core/Property.hpp"
#include "../../include/core/Player.hpp"
#include "../../include/logic/Game.hpp"

#include <cmath>

using namespace std;

namespace core {

Railroad::Railroad(string name, int mortgageValue, int baseFare) : Property(move(name), 0, mortgageValue), baseFare_(baseFare) {}

int Railroad::calculateRent(logic::Game& g) {
    if (isMortgagedStatus()) {
        return 0;
    } 

    Player* owner = getOwner();
    if (!owner) {
        return 0;
    }

    auto properties = g.getPropertiesOwnedBy(owner);
    int ownedRailroads = 0;
    for (auto* prop : properties) {
        if (prop->getType() == "RAILROAD" && !prop->isMortgagedStatus()) {
            ownedRailroads++;
        }
    }

    return static_cast<int>(baseFare_ * pow(2, ownedRailroads - 1));
}

string Railroad::getType() const { 
    return "RAILROAD"; 
}

int Railroad::getBaseFare() const { 
    return baseFare_; 
}

}