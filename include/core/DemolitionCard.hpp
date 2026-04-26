#pragma once

#include <memory>
#include <string>

#include <core/Player.hpp>
#include <core/SkillCard.hpp>

// TODO: this class has out-of-spec method(s).

namespace core {

class GameContext;

/**
 * @brief Skill that attempts to destroy an opponent building (stub hook).
 */
class DemolitionCard : public SkillCard {
public:
    /**
     * @brief Construct with flavour text.
     * @param description Human-readable description for UI logs.
     */
    explicit DemolitionCard(std::string description);

    // TODO: extend to hotels and multi-house removal when UI is ready.
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
