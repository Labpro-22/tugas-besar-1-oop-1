#include "core/DiscountCard.hpp"

#include "logic/Game.hpp"

#include <random>
#include <utility>

namespace core {

DiscountCard::DiscountCard(float discountRate, std::string description)
    : SkillCard(std::move(description)), discountRate_(discountRate) {}

void DiscountCard::execute(Player& player, logic::Game& game) {
    (void)game;
    player.consumeSkillUse();
    player.applyDiscount(discountRate_);
}

std::string DiscountCard::getCardType() const { return "Discount"; }

std::unique_ptr<ActionCard> DiscountCard::makeRandom(std::string description) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.05F, 0.50F);
    const float rate = dist(gen);
    return std::unique_ptr<ActionCard>(new DiscountCard(rate, std::move(description)));
}

}
