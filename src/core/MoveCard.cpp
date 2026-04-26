#include "core/card/MoveCard.hpp"

#include <utility>

#include "core/GameContext.hpp"

namespace core {

MoveCard::MoveCard(int steps, std::string description)
    : SkillCard(std::move(description)), steps_(steps) {}

void MoveCard::execute(Player& player, GameContext& ctx) {
  player.consumeSkillUse();  // TODO: this method is not in the spec. Might fix
                             // this later.
  const int n = ctx.getBoardSize();
  if (n <= 0) {
    return;
  }
  // TODO: out-of-spec - bypasses Game::moveCurrentPlayer / onPassed / pass-Go
  // handling.
  int pos = player.getPosition() + steps_;
  pos %= n;
  if (pos < 0) {
    pos += n;
  }
  player.setPosition(pos);
}

std::string MoveCard::getCardType() const { return "Move"; }

std::unique_ptr<ActionCard> MoveCard::make(int steps, std::string description) {
  return std::unique_ptr<ActionCard>(
      new MoveCard(steps, std::move(description)));
}

}  // namespace core
