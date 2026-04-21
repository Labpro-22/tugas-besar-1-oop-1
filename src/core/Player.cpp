#include "core/Player.hpp"

namespace core {

Player::Player(string name) : name_(name), balance_(0), position_(1), inJail_(false), isBankrupt_(false), jailTurns_(0) {}

void Player::addProperty(Property* p) {
    ownedProperties_.push_back(p);
}

void Player::goToJail() {
    inJail_ = true;
    position_ = 11; // random index
}

int Player::getBalance() const {
    return balance_;
}

bool Player::isHuman() { return true; }
void Player::takeTurn(logic::Game& g) {}
void Player::releaseFromJail() { inJail_ = false; }
void Player::removeProperty(Property* p) {}
int Player::getNetWorth() const { return balance_; }
bool Player::canAfford(int amount) const { return balance_ >= amount; }
string Player::getName() const { return name_; }
int Player::getPosition() const { return position_; }
bool Player::getInJail() const { return inJail_; }
bool Player::getIsBankrupt() const { return isBankrupt_; }
vector<Property*>& Player::getOwnedProperties() { return ownedProperties_; }

Player& Player::operator+(int amount) { balance_ += amount; return *this; }
Player& Player::operator-=(int amount) { balance_ -= amount; return *this; }
bool Player::operator<(const Player& other) const { return balance_ < other.balance_; }
bool Player::operator>(const Player& other) const { return balance_ > other.balance_; }
bool Player::operator==(const Player& other) const { return name_ == other.name_; }
} // namespace core
