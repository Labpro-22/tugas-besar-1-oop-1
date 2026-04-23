#pragma once

#include <memory>
#include <string>

#include <core/Player.hpp>
#include <core/SkillCard.hpp>

namespace logic {

class Game;

}

namespace core {

/**
 * @brief Skill that advances the current player a relative number of spaces.
 */
class MoveCard : public SkillCard {
public:
    /**
     * @brief Capture the relative movement amount.
     * @param steps Number of clockwise spaces to travel.
     * @param description Human-readable description for UI logs.
     */
    MoveCard(int steps, std::string description);

    void execute(Player& player, logic::Game& game) override;

    std::string getCardType() const override;

    /**
     * @brief Factory helper for deck loaders.
     * @param steps Relative movement forwarded to `Game::moveCurrentPlayer`.
     * @param description Human-readable description for UI logs.
     * @note Extension (not in spec).
     */
    static std::unique_ptr<ActionCard> make(int steps, std::string description);
     // IDK why I made this :D

private:
    int steps_;
};

}
