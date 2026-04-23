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
 * @brief Skill that attempts to destroy an opponent building (stub hook).
 */
class DemolitionCard : public SkillCard {
public:
    /**
     * @brief Construct with flavour text.
     * @param description Human-readable description for UI logs.
     */
    explicit DemolitionCard(std::string description);

    // TODO: this method doesn't actually do anything for now.
    // FIXME: make this method actually demolish a property.
    void execute(Player& player, logic::Game& game) override;

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
