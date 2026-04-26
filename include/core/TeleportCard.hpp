#pragma once

#include <memory>
#include <string>

#include <core/Player.hpp>
#include <core/SkillCard.hpp>

// TODO: this class has out-of-spec method(s).

namespace core {

class GameContext;

/**
 * @brief Skill that jumps the activator to an absolute board index.
 */
class TeleportCard : public SkillCard {
public:
    /**
     * @brief Capture the destination index and flavour text.
     * @param targetPosition Zero-based tile index on the configured board.
     * @param description Human-readable description for UI logs.
     */
    TeleportCard(int targetPosition, std::string description);

    void execute(Player& player, GameContext& context) override;

    std::string getCardType() const override;

    /**
     * @brief Factory returning a heap-allocated teleport skill.
     * @param targetPosition Absolute index passed to `Player::setPosition`.
     * @param description Human-readable description for UI logs.
     * @note Extension (not in spec).
     */
    static std::unique_ptr<ActionCard> make(int targetPosition, std::string description);
    // same functionality as make in ChanceCard.cpp and ShieldCard.cpp, also makeRandom in DiscountCard.cpp

private:
    int targetPosition_;
};

}
