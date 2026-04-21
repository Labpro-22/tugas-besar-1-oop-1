#include "include/core/Property.hpp"
#include "include/core/GameException.hpp"

using namespace std;

namespace core{

Property::Property() : name_(""), owner_(nullptr), isMortgaged_(false), price_(0), mortgageValue_(0), festMultiplier_(1), festDuration_(0) {}

Property::Property(const string& name, int price, int mortgageValue) : name_(name), owner_(nullptr), isMortgaged_(false), price_(price), mortgageValue_(mortgageValue), festMultiplier_(1), festDuration_(0) {}

void Property::mortgage() {
    if(!isOwned()) {
        throw InvalidMoveException("Tidak bisa menggadaikan properti yang belum dimiliki");
    }
    if(isMortgaged_){
        throw InvalidMoveException("Properti '" + name_ + "' sudah dalam status tergadai");
    }
    isMortgaged_ = true;
}; 

void Property::unmortgage(){
    if(!isMortgaged_){
        throw InvalidMoveException("Properti '" + name_ +  "' tidak sedang digadaikan");
    }
    isMortgaged_ = false;
}; 

void Property::setOwner(Player* p){
    owner_ = p;
}; 

Player* Property::getOwner() const{
    return owner_;
}; 

bool Property::isOwned() const{
    return (owner_ != nullptr);
}; 

bool Property::isAvailable() const{
    return (owner_ == nullptr);
}; 

bool Property::isMortgagedStatus() const{
    return isMortgaged_;
};

const string& Property::getName() const{
    return name_;
};

int Property::getPrice() const{
    return price_;
};
int Property::getMortgageValue() const{
    return mortgageValue_;
}; 

void Property::applyFestival() {
    if (festMultiplier_ < 8) {
        festMultiplier_ *= 2;
    }
    festDuration_ = 3;
}

void Property::tickFestival() {
    if (festDuration_ <= 0) return;

    --festDuration_;
    if (festDuration_ == 0){
        festMultiplier_ = 1;
    }
}

int Property::getFestMultiplier() const {
    return festMultiplier_;
}

int Property::getFestDuration() const {
    return festDuration_;
}

void Property::setFestivalState(int mult, int dur) {
    if (mult != 1 && mult != 2 && mult != 4 && mult != 8){
        throw InvalidConfigException("festivalMultiplier", "1, 2, 4, atau  8");
    }
    if (dur < 0 || dur > 3){
        throw InvalidConfigException("festivalDuration", "antara 0 dan 3");
    }
    festMultiplier_ = mult;
    festDuration_   = dur;
}

}