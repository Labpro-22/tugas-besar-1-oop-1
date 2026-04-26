#pragma once

#include <memory>
#include <string>

#include <core/Player.hpp>
#include <core/SkillCard.hpp>

namespace core {

class GameContext;

/**
 * @brief Skill that pulls the next opponent clockwise onto the activator tile.
 */
class LassoCard : public SkillCard {
 public:
    /**
     * @brief Construct with flavour text only (geometry comes from live state).
     * @param description Human-readable description for UI logs.
     */
    explicit LassoCard(std::string description);

    void execute(Player& player, GameContext& context) override;

    std::string getCardType() const override;

    /**
     * @brief Factory helper for deck loaders.
     * @param description Human-readable description for UI logs.
     * @note Extension (not in spec).
     */
    static std::unique_ptr<ActionCard> make(std::string description);
    // same functionality as make in ChanceCard.cpp and ShieldCard.cpp, also makeRandom in DiscountCard.cpp
};

}
