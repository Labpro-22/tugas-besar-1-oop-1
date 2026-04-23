#include "core/LassoCard.hpp"

#include "logic/Game.hpp"

#include <limits>
#include <utility>

namespace core {

LassoCard::LassoCard(std::string description)
    : SkillCard(std::move(description)) {}

void LassoCard::execute(Player& player, logic::Game& game) {
    player.consumeSkillUse();
    const int myPos = player.getPosition();
    Player* victim = nullptr;
    int best = std::numeric_limits<int>::max();
    
    // search for the closest player with position index greater than the current player's position
    for (Player* candidate : game.players()) {
        if (candidate == nullptr || candidate == &player) {
            continue;
        }
        const int pos = candidate->getPosition();
        if (pos > myPos && pos < best) {
            best = pos;
            victim = candidate;
        }
    }
    // if no player is found, then that means all other players are behind the current player.
    if (victim == nullptr) {
        best = std::numeric_limits<int>::max();
        for (Player* candidate : game.players()) {
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
        victim->setPosition(myPos); // TODO: this is out-of-spec method. IDK if there are alternatives to handle this.
    }
}

std::string LassoCard::getCardType() const { return "Lasso"; }

std::unique_ptr<ActionCard> LassoCard::make(std::string description) {
    return std::unique_ptr<ActionCard>(new LassoCard(std::move(description)));
}

}
