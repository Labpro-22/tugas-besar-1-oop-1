#include "core/card/ShieldCard.hpp"

#include <utility>

#include "core/GameContext.hpp"
#include "data/LogEntry.hpp"

namespace core {

ShieldCard::ShieldCard(std::string description)
    : SkillCard(std::move(description)) {}

void ShieldCard::execute(Player& player, GameContext& ctx) {
  player.consumeSkillUse();
  player.useShield();
  ctx.logEvent(data::LogAction::SPECIAL_CARD_USE, player, 0);
}

std::string ShieldCard::getCardType() const { return "Shield"; }

std::unique_ptr<ActionCard> ShieldCard::make(std::string description) {
  return std::unique_ptr<ActionCard>(new ShieldCard(std::move(description)));
}

}  // namespace core
