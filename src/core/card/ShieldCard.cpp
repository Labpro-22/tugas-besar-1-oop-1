#include "core/card/ShieldCard.hpp"

#include <utility>

#include "core/GameContext.hpp"

namespace core {

ShieldCard::ShieldCard(std::string description)
    : SkillCard(std::move(description)) {}

void ShieldCard::execute(Player& player, GameContext& ctx) {
  (void)ctx;
  player.consumeSkillUse();
  player.useShield();
}

std::string ShieldCard::getCardType() const { return "Shield"; }

std::unique_ptr<ActionCard> ShieldCard::make(std::string description) {
  return std::unique_ptr<ActionCard>(new ShieldCard(std::move(description)));
}

}  // namespace core
