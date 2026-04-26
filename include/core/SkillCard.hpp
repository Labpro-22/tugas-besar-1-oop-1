#pragma once

#include <string>

#include <core/ActionCard.hpp>

namespace core {

/**
 * @brief Intermediate abstract type for skill cards held in the player's hand.
 *
 * Concrete subclasses implement the colourful effects described in the
 * specification while sharing construction rules with `ActionCard`.
 */
class SkillCard : public ActionCard {
protected:
    /**
     * @brief Forward the flavour text to `ActionCard`.
     * @param description Human-readable card description.
     */
    explicit SkillCard(std::string description);
};

}
