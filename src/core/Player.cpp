#include "core/Player.hpp"

#include "core/ActionCard.hpp"
#include "core/GameException.hpp"
#include "core/Property.hpp"

#include <algorithm>
#include <utility>

namespace core {

Player::Player(std::string name, std::string token)
        :   name_(std::move(name)),
            token_(std::move(token)),
            balance_(0),
            position_(0),
            inJail_(false),
            isBankrupt_(false),
            jailTurns_(0),
            ownedProperties_(),
            heldCards_(),
            shieldActive_(false),
            usedSkillThisTurn_(false),
            discountRate_(0.0F) {}

int Player::getNetWorth() const noexcept {
    int total = balance_;
    for (Property* p : ownedProperties_) {
        if (p != nullptr) {
            total += p->isMortgagedStatus() ? p->getMortgageValue() : p->getPrice();
        }
    }
    return total;
}

bool Player::canAfford(int amount) const noexcept {
    return balance_ >= amount;
}

void Player::goToJail() { inJail_ = true; }

void Player::releaseFromJail() {
    inJail_ = false;
    jailTurns_ = 0;
}

void Player::addProperty(Property* property) {
    if (property == nullptr) {
        return;
    }
    ownedProperties_.push_back(property);
}

void Player::removeProperty(Property* property) {
    const auto it = std::find(ownedProperties_.begin(), ownedProperties_.end(), property);
    if (it != ownedProperties_.end()) {
        ownedProperties_.erase(it);
    }
}

Player& Player::operator+=(int money) {
    balance_ += money;
    return *this;
}

Player& Player::operator-=(int money) {
    balance_ -= money;
    return *this;
}

bool Player::operator<(const Player& other) const noexcept {
    return getNetWorth() < other.getNetWorth();
}

bool Player::operator>(const Player& other) const noexcept {
    return getNetWorth() > other.getNetWorth();
}

bool Player::operator==(const Player& other) const noexcept {
    return getNetWorth() == other.getNetWorth();
}

const std::string& Player::getName() const noexcept { return name_; }

const std::string& Player::getToken() const noexcept { return token_; }

int Player::getBalance() const noexcept { return balance_; }

int Player::getPosition() const noexcept { return position_; }

void Player::setPosition(int index) noexcept { position_ = index; }

bool Player::isInJail() const noexcept { return inJail_; }

bool Player::isBankrupted() const noexcept { return isBankrupt_; }

void Player::setBankrupted(bool value) noexcept { isBankrupt_ = value; }

void Player::addCard(ActionCard* card) {
    if (heldCards_.size() >= 3U) {
        throw InvalidMoveException("Player hand already holds three action cards.");
    }
    if (card == nullptr) {
        return;
    }
    heldCards_.push_back(card);
}

void Player::removeCard(ActionCard* card) {
    const auto it = std::find(heldCards_.begin(), heldCards_.end(), card);
    if (it != heldCards_.end()) {
        heldCards_.erase(it);
    }
}

void Player::useShield() { shieldActive_ = true; }

bool Player::isShielded() const noexcept { return shieldActive_; }

void Player::consumeSkillUse() {
    if (usedSkillThisTurn_) {
        throw InvalidMoveException("Skill card already used this turn.");
    }
    usedSkillThisTurn_ = true;
}

void Player::resetPerTurnFlags() noexcept {
    shieldActive_ = false;
    usedSkillThisTurn_ = false;
    discountRate_ = 0.0F;
}

void Player::applyDiscount(float rate) noexcept { discountRate_ = rate; }

int Player::promptChoice(const std::string& /*context*/, int defaultIndex, int /*optionCount*/) {
    return defaultIndex;
}

const std::vector<Property*>& Player::getOwnedProperties() const noexcept {
    return ownedProperties_;
}

const std::vector<ActionCard*>& Player::getHeldCards() const noexcept {
    return heldCards_;
}

int Player::getJailTurns() const noexcept { return jailTurns_; }

void Player::incrementJailTurns() noexcept { ++jailTurns_; }

void Player::clearJailTurns() noexcept { jailTurns_ = 0; }

void Player::setBalance(int amount) noexcept { balance_ = amount; }

}
