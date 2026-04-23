#include "core/DemolitionCard.hpp"

#include "logic/Game.hpp"

#include <cstddef>
#include <vector>
#include <utility>

namespace core {

DemolitionCard::DemolitionCard(std::string description)
    : SkillCard(std::move(description)) {}

void DemolitionCard::execute(Player& player, logic::Game& game) {
    player.consumeSkillUse();
    const std::vector<core::Player*>& participants = game.players();
    const int optionCount = static_cast<int>(participants.size());
    // TODO: the expression below uses out-of-spec method. IDK how to handle this using only the defined methods in the spec.
    const int choice = player.promptChoice("DemolitionVictim", 0, optionCount);
    if (choice < 0 || choice >= optionCount) {
        return;
    }
    Player* victim = participants[static_cast<std::size_t>(choice)];
    if (victim == nullptr || victim == &player) {
        return;
    }
    if (victim->getOwnedProperties().empty()) {
        game.logEvent("DEMOLITION_MISS", player, 0);
        return;
    }
    game.logEvent("DEMOLITION", player, 0);
}

std::string DemolitionCard::getCardType() const { return "Demolition"; }

std::unique_ptr<ActionCard> DemolitionCard::make(std::string description) {
    return std::unique_ptr<ActionCard>(new DemolitionCard(std::move(description)));
}

}
