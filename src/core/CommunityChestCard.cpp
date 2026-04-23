#include "core/CommunityChestCard.hpp"

#include "core/Player.hpp"
#include "logic/Bank.hpp"
#include "logic/Game.hpp"

#include <utility>

namespace core {

CommunityChestCard::CommunityChestCard(std::string description, std::function<void(Player&, logic::Game&)> effect)
        : ActionCard(std::move(description)), effect_(std::move(effect)) {}

void CommunityChestCard::execute(Player& player, logic::Game& game) {
    if (effect_) {
        effect_(player, game);
    }
}

std::string CommunityChestCard::getCardType() const {
    return "CommunityChest";
}

std::unique_ptr<ActionCard> CommunityChestCard::makePayBank(int amount, std::string description) {
    return std::unique_ptr<ActionCard>(new CommunityChestCard(std::move(description),  [amount](Player& player, logic::Game& game) {
            player -= amount;
            game.bank().receive(amount);
        }));
}

std::unique_ptr<ActionCard> CommunityChestCard::makeCollect(int amount, std::string description) {
    return std::unique_ptr<ActionCard>(new CommunityChestCard(std::move(description), [amount](Player& player, logic::Game& game) {
            player += amount;
            game.bank().pay(player, amount);
        }));
}

}
