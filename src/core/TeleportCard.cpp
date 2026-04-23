#include "core/TeleportCard.hpp"

#include "logic/Game.hpp"

#include <utility>

namespace core {

TeleportCard::TeleportCard(int targetPosition, std::string description)
    : SkillCard(std::move(description)), targetPosition_(targetPosition) {}

void TeleportCard::execute(Player& player, logic::Game& game) {
    (void)game;
    player.consumeSkillUse();
    player.setPosition(targetPosition_);
}

std::string TeleportCard::getCardType() const { return "Teleport"; }

std::unique_ptr<ActionCard> TeleportCard::make(int targetPosition, std::string description) {
    return std::unique_ptr<ActionCard>(new TeleportCard(targetPosition, std::move(description)));
}

}
