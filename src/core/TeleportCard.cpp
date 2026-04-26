#include "core/card/TeleportCard.hpp"

#include <utility>

#include "core/GameContext.hpp"

namespace core {

TeleportCard::TeleportCard(int targetPosition, std::string description)
    : SkillCard(std::move(description)), targetPosition_(targetPosition) {}

void TeleportCard::execute(Player& player, GameContext& ctx) {
  player.consumeSkillUse();
  const int n = ctx.getBoardSize();
  if (n <= 0) {
    return;
  }
  // TODO: out-of-spec - bypasses Game::moveCurrentPlayer / onPassed / pass-Go
  // handling.
  int idx = targetPosition_ % n;
  if (idx < 0) {
    idx += n;
  }
  player.setPosition(idx);
}

std::string TeleportCard::getCardType() const { return "Teleport"; }

std::unique_ptr<ActionCard> TeleportCard::make(int targetPosition,
                                               std::string description) {
  return std::unique_ptr<ActionCard>(
      new TeleportCard(targetPosition, std::move(description)));
}

}  // namespace core
