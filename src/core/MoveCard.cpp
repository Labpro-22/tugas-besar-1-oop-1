#include "core/MoveCard.hpp"

#include "logic/Game.hpp"

#include <utility>

namespace core {

MoveCard::MoveCard(int steps, std::string description)
    : SkillCard(std::move(description)), steps_(steps) {}

void MoveCard::execute(Player& player, logic::Game& game) {
    player.consumeSkillUse(); // TODO: this method is not in the spec. Might fix this later.
    game.moveCurrentPlayer(steps_);
}

std::string MoveCard::getCardType() const { return "Move"; }

std::unique_ptr<ActionCard> MoveCard::make(int steps, std::string description) {
    return std::unique_ptr<ActionCard>(new MoveCard(steps, std::move(description)));
}

}
