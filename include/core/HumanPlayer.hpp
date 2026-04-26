#pragma once

#include <core/Player.hpp>

namespace core {

/**
 * @brief Player implementation that blocks on UI events instead of automating.
 */
class HumanPlayer : public Player {
public:
    /**
     * @brief Construct a human-controlled seat.
     * @param name Display name persisted in saves/logs.
     * @param token ASCII marker rendered on the board.
     */
    HumanPlayer(std::string name, std::string token);

    ~HumanPlayer() override = default;

    void takeTurn(logic::Game& game) override;

    bool isHuman() override;
};

}
