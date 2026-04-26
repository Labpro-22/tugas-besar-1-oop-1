#include "core/card/TeleportCard.hpp"

#include <utility>

#include "core/GameContext.hpp"
#include "core/player/Player.hpp"
#include "data/LogEntry.hpp"

namespace core {

TeleportCard::TeleportCard(int targetPosition, std::string description)
    : SkillCard(std::move(description)), targetPosition_(targetPosition) {}

void TeleportCard::execute(Player& player, GameContext& ctx) {
  player.consumeSkillUse();
  const int n = ctx.getBoardSize();
  if (n <= 0) {
    return;
  }
  int idx = targetPosition_ % n;
  if (idx < 0) {
    idx += n;
  }
  ctx.logEvent(data::LogAction::SPECIAL_CARD_USE, player, idx);
  ctx.teleportPlayer(player, idx);
}

std::string TeleportCard::getCardType() const { return "Teleport"; }

std::unique_ptr<ActionCard> TeleportCard::make(int targetPosition,
                                               std::string description) {
  return std::unique_ptr<ActionCard>(
      new TeleportCard(targetPosition, std::move(description)));
}

}  // namespace core
