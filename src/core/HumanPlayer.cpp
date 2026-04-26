#include "core/HumanPlayer.hpp"
#include "logic/Game.hpp"

#include <utility>

namespace core {

HumanPlayer::HumanPlayer(std::string name, std::string token)
        : Player(std::move(name), std::move(token)) {}

void HumanPlayer::takeTurn(logic::Game& game) {
    resetPerTurnFlags();
    (void)game;
    // UI layer is responsible for issuing commands; this stub simply yields.
}

bool HumanPlayer::isHuman() { return true; }

}
