#include "core/MoveCard.hpp"

#include "core/GameContext.hpp"
#include "core/Player.hpp"

#include <utility>

namespace core {

MoveCard::MoveCard(int steps, std::string description)
    : SkillCard(std::move(description)), steps_(steps) {}

void MoveCard::execute(Player& player, GameContext& ctx) {
    player.consumeSkillUse();
    const int n = ctx.getBoardSize();
    if (n <= 0) {
        return;
    }
    int target = player.getPosition() + steps_;
    target %= n;
    if (target < 0) {
        target += n;
    }
    ctx.movePlayer(player, target);
}

std::string MoveCard::getCardType() const { return "Move"; }

std::unique_ptr<ActionCard> MoveCard::make(int steps, std::string description) {
    return std::unique_ptr<ActionCard>(new MoveCard(steps, std::move(description)));
}

}
