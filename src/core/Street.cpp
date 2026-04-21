#include "../../include/core/Property.hpp"
#include "../../include/core/Player.hpp"
#include "../../include/core/GameException.hpp"
#include "../../include/logic/Game.hpp"

#include <algorithm>

namespace core {

Street::Street(string name, int price, int mortgageValue, ColorGroup colorGroup, vector<int> rentValues, int houseCost, int hotelCost)
    : Property(move(name), price, mortgageValue), colorGroup_(colorGroup), houseCount_(0), hotelCount_(0), rentValues_(move(rentValues)), houseCost_(houseCost), hotelCost_(hotelCost), festivalMultiplier_(1), festivalDuration_(0){
    if (rentValues_.size() != 6) {
        throw InvalidConfigException("rentValues untuk '" + getName() + "'", "tepat 6 elemen (level 0-5)");
    }
}

int Street::calculateRent(logic::Game& g) {
    if (isMortgagedStatus()) {
        return 0;
    }

    int rent;
    if (hotelCount_ > 0) {
        rent = rentValues_[5];
    } else if (houseCount_ > 0) {
        rent = rentValues_[houseCount_];
    } else if (hasMonopoly(g)) {
        rent = rentValues_[0] * 2;
    } else {
        rent = rentValues_[0];
    }

    rent *= festivalMultiplier_;
    return rent;
}

string Street::getType() const { 
    return "STREET"; 
}

void Street::buildHouse() {
    if (hotelCount_ > 0){
        throw InvalidMoveException("Petak '" + getName() + "' sudah memiliki hotel");
    }
    if (houseCount_ >= 4){
        throw InvalidMoveException("Jumlah rumah di '" + getName() + "' sudah maksimal (4)");
    }
    houseCount_++;
}

void Street::buildHotel() {
    if (houseCount_ < 4){
        throw InvalidMoveException("Harus memiliki 4 rumah sebelum upgrade ke hotel di '" + getName() + "'");
    }
    if (hotelCount_ > 0) {
        throw InvalidMoveException("Petak '" + getName() + "' sudah memiliki hotel");
    }
    houseCount_ = 0;
    hotelCount_ = 1;
}

void Street::demolish(int n) {
    if (n <= 0) return;
    if (hotelCount_ > 0) {
        hotelCount_ = 0;
        n--;
    }
    int toRemove = min(n, houseCount_);
    houseCount_ -= toRemove;
}


void Street::applyFestival() {
    if (festivalMultiplier_ < 8) {
        festivalMultiplier_ *= 2;
    }
    festivalDuration_ = 3;
}

void Street::tickFestival() {
    if (festivalDuration_ <= 0) return;

    festivalDuration_--;

    if (festivalDuration_ == 0) {
        festivalMultiplier_ = 1;
    }
}


bool Street::canBuild(logic::Game& g) const {
    if (!hasMonopoly(g)) {
        return false;
    }
    
    if (hotelCount_ > 0) {
        return false;
    }

    return true;
}

bool Street::hasMonopoly(logic::Game& g) const {
    if (!isOwned()) {
        return false;
    }

    Player* owner = getOwner();
    auto properties = g.getPropertiesOwnedBy(owner);

    int ownedInGroup = 0;
    for (auto* prop : properties) {
        if (prop->getType() != "STREET") continue;
        Street* s = static_cast<Street*>(prop);
        if (s->getColorGroup() == colorGroup_) {
            ownedInGroup++;
        }
    }

    int totalInGroup = (colorGroup_ == ColorGroup::COKLAT || colorGroup_ == ColorGroup::BIRU_TUA) ? 2 : 3;
    return ownedInGroup == totalInGroup;
}


ColorGroup Street::getColorGroup() const { return colorGroup_; }
int Street::getHouseCount() const { 
    return houseCount_; 
}
int Street::getHotelCount() const { 
    return hotelCount_; 
}
int Street::getFestivalMultiplier() const { 
    return festivalMultiplier_; 
}
int Street::getFestivalDuration() const { 
    return festivalDuration_; 
}

void Street::setFestivalState(int mult, int dur) {
    if (mult != 1 && mult != 2 && mult != 4 && mult != 8) {
        throw InvalidConfigException("festivalMultiplier", "1, 2, 4, atau 8");
    }

    if (dur < 0 || dur > 3) { 
        throw InvalidConfigException("festivalDuration", "antara 0-3");
    }

    festivalMultiplier_ = mult;
    festivalDuration_   = dur;
}

}