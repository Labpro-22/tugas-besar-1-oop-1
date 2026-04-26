#include "core/card/DemolitionCard.hpp"

#include <cstddef>
#include <utility>
#include <vector>

#include "core/GameContext.hpp"
#include "core/Property.hpp"

namespace core {

DemolitionCard::DemolitionCard(std::string description)
    : SkillCard(std::move(description)) {}

void DemolitionCard::execute(Player& player, GameContext& ctx) {
  player.consumeSkillUse();
  const std::vector<Player*>& participants = ctx.getPlayers();
  const int optionCount = static_cast<int>(participants.size());
  // TODO: out-of-spec - `promptChoice` is not in the course spec; needed until
  // UI exposes targets.
  const int choice = player.promptChoice("DemolitionVictim", 0, optionCount);
  if (choice < 0 || choice >= optionCount) {
    return;
  }
  Player* victim = participants[static_cast<std::size_t>(choice)];
  if (victim == nullptr || victim == &player) {
    return;
  }
  const std::vector<Property*>& props = victim->getOwnedProperties();
  if (props.empty()) {
    ctx.logEvent("DEMOLITION_MISS", player, 0);
    return;
  }
  const int propChoice = player.promptChoice("DemolitionProperty", 0,
                                             static_cast<int>(props.size()));
  if (propChoice < 0 || propChoice >= static_cast<int>(props.size())) {
    return;
  }
  Property* prop = props[static_cast<std::size_t>(propChoice)];
  if (prop == nullptr) {
    ctx.logEvent("DEMOLITION_MISS", player, 0);
    return;
  }
  if (prop->getType() != PropertyType::STREET) {
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
  return std::unique_ptr<ActionCard>(
      new DemolitionCard(std::move(description)));
}

}  // namespace core
