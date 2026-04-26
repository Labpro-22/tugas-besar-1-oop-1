#include "core/card/LassoCard.hpp"

#include <limits>
#include <utility>

#include "core/GameContext.hpp"

namespace core {

LassoCard::LassoCard(std::string description)
    : SkillCard(std::move(description)) {}

void LassoCard::execute(Player& player, GameContext& ctx) {
  player.consumeSkillUse();
  const int myPos = player.getPosition();
  Player* victim = nullptr;
  int best = std::numeric_limits<int>::max();

  // Prefer the closest player strictly ahead of the caster on the board.
  for (Player* candidate : ctx.getPlayers()) {
    if (candidate == nullptr || candidate == &player) {
      continue;
    }
    const int pos = candidate->getPosition();
    if (pos > myPos && pos < best) {
      best = pos;
      victim = candidate;
    }
  }
  // Fall back to the lowest-positioned opponent when everyone else is behind
  // us.
  if (victim == nullptr) {
    best = std::numeric_limits<int>::max();
    for (Player* candidate : ctx.getPlayers()) {
      if (candidate == nullptr || candidate == &player) {
        continue;
      }
      const int pos = candidate->getPosition();
      if (pos < best) {
        best = pos;
        victim = candidate;
      }
    }
  }
  if (victim != nullptr) {
    // TODO: out-of-spec - `setPosition` is not named in the course spec; no
    // GameContext hook for lasso yet.
    victim->setPosition(myPos);
  }
}

std::string LassoCard::getCardType() const { return "Lasso"; }

std::unique_ptr<ActionCard> LassoCard::make(std::string description) {
  return std::unique_ptr<ActionCard>(new LassoCard(std::move(description)));
}

}  // namespace core
