#include "core/LassoCard.hpp"

#include "core/GameContext.hpp"

#include <limits>
#include <utility>

namespace core {

LassoCard::LassoCard(std::string description)
    : SkillCard(std::move(description)) {}

void LassoCard::execute(Player& player, GameContext& ctx) {
    player.consumeSkillUse();
    const int myPos = player.getPosition();
    Player* victim = nullptr;
    int best = std::numeric_limits<int>::max();
    
    // search for the closest player with position index greater than the current player's position
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
    // if no player is found, then that means all other players are behind the current player.
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
        // TODO: out-of-spec - `setPosition` is not named in the course spec; no GameContext hook for lasso yet.
        victim->setPosition(myPos);
    }
}

std::string LassoCard::getCardType() const { return "Lasso"; }

std::unique_ptr<ActionCard> LassoCard::make(std::string description) {
    return std::unique_ptr<ActionCard>(new LassoCard(std::move(description)));
}

}
