#include "core/DemolitionCard.hpp"

#include "core/GameContext.hpp"
#include "core/Property.hpp"

#include <cstddef>
#include <utility>
#include <vector>

namespace core {

DemolitionCard::DemolitionCard(std::string description)
    : SkillCard(std::move(description)) {}

void DemolitionCard::execute(Player& player, GameContext& ctx) {
    player.consumeSkillUse();

    std::vector<Player*> victims;
    victims.reserve(ctx.getPlayers().size());
    for (Player* candidate : ctx.getPlayers()) {
        if (candidate == nullptr || candidate == &player) {
            continue;
        }
        victims.push_back(candidate);
    }
    if (victims.empty()) {
        ctx.logEvent("DEMOLITION_MISS", player, 0);
        return;
    }

    const int victimChoice = player.promptChoice("DemolitionVictim", 0, static_cast<int>(victims.size()));
    if (victimChoice < 0 || victimChoice >= static_cast<int>(victims.size())) {
        return;
    }
    Player* victim = victims[static_cast<std::size_t>(victimChoice)];

    const std::vector<Property*>& props = victim->getOwnedProperties();
    if (props.empty()) {
        ctx.logEvent("DEMOLITION_MISS", player, 0);
        return;
    }
    const int propChoice = player.promptChoice("DemolitionProperty", 0, static_cast<int>(props.size()));
    if (propChoice < 0 || propChoice >= static_cast<int>(props.size())) {
        return;
    }
    Property* prop = props[static_cast<std::size_t>(propChoice)];
    if (prop == nullptr || prop->getType() != PropertyType::STREET) {
        ctx.logEvent("DEMOLITION_MISS", player, 0);
        return;
    }
    auto* street = static_cast<Street*>(prop);
    if (street->getHouseCount() == 0 && street->getHotelCount() == 0) {
        ctx.logEvent("DEMOLITION_MISS", player, 0);
        return;
    }
    street->demolish(1);
    ctx.logEvent("DEMOLITION", player, *prop, 0);
}

std::string DemolitionCard::getCardType() const { return "Demolition"; }

std::unique_ptr<ActionCard> DemolitionCard::make(std::string description) {
    return std::unique_ptr<ActionCard>(new DemolitionCard(std::move(description)));
}

}
