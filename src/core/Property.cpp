#include "../../include/core/Property.hpp"
#include "../../include/core/Player.hpp"
#include "../../include/logic/Game.hpp"
#include "../../include/core/GameException.hpp"
#include <algorithm>
#include <cmath>

using namespace std;

namespace core{

Property::Property() : name_(""), owner_(nullptr), isMortgaged_(false), price_(0), mortgageValue_(0) {};

Property::Property(string name, int price, int mortgageValue) : name_(move(name)), owner_(nullptr), isMortgaged_(false), price_(price), mortgageValue_(mortgageValue) {};

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

string Property::getName() const{
    return name_;
};
int Property::getPrice() const{
    return price_;
};
int Property::getMortgageValue() const{
    return mortgageValue_;
}; 

}