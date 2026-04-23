#pragma once

#include <memory>
#include <string>

#include <core/Player.hpp>
#include <core/SkillCard.hpp>

namespace logic {

class Game;

}

// TODO: this class has out-of-spec method(s).

namespace core {

/**
 * @brief Skill that flags the player as immune to harmful payments briefly.
 */
class ShieldCard : public SkillCard {
 public:
    /**
     * @brief Construct with flavour text.
     * @param description Human-readable description for UI logs.
     */
    explicit ShieldCard(std::string description);

    void execute(Player& player, logic::Game& game) override;

    std::string getCardType() const override;

    /**
     * @brief Factory helper for deck loaders.
     * @param description Human-readable description for UI logs.
     * @note Extension (not in spec).
     */
    static std::unique_ptr<ActionCard> make(std::string description);
    // same functionality as make in ChanceCard.cpp and makeRandom in DiscountCard.cpp
};

}
