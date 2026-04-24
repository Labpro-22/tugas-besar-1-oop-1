#pragma once

#include <memory>
#include <string>

#include <core/Player.hpp>
#include <core/SkillCard.hpp>

// TODO: this class has out-of-spec method(s).

namespace core {

class GameContext;

/**
 * @brief Skill that rolls a random discount percentage when acquired.
 */
class DiscountCard : public SkillCard {
public:
    /**
     * @brief Store the rolled discount multiplier.
     * @param discountRate Fractional discount between `0` and `1`.
     * @param description Human-readable description for UI logs.
     */
    DiscountCard(float discountRate, std::string description);

    void execute(Player& player, GameContext& context) override;

    std::string getCardType() const override;

    /**
     * @brief Factory that rolls a discount in `[0.05, 0.50]`.
     * @param description Human-readable description for UI logs.
     * @note Extension (not in spec).
     */
    static std::unique_ptr<ActionCard> makeRandom(std::string description);
    // same functionality as make in ChanceCard.cpp

private:
    float discountRate_;
};

}
