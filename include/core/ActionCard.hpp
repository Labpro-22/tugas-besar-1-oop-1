#pragma once

#include <string>

namespace logic {

class Game;

}

namespace core {

class Player;

/**
 * @brief Abstract root for every drawable card (Chance, Community Chest, Skill).
 *
 * Cards are owned by deck objects (outside this module) as
 * `std::unique_ptr<ActionCard>`; a player's hand stores non-owning pointers.
 */
class ActionCard {
public:
    /**
     * @brief Construct a card with human-readable text.
     * @param description Text shown when the card is revealed.
     */
    explicit ActionCard(std::string description);

    ActionCard(const ActionCard&) = delete;
    ActionCard& operator=(const ActionCard&) = delete;
    ActionCard(ActionCard&&) = delete;
    ActionCard& operator=(ActionCard&&) = delete;

    virtual ~ActionCard() = default;

    /**
     * @brief Apply this card's effect to @p player within @p game.
     * @param player The subject of the effect (drawer or activator).
     * @param game Live game state used to mutate board/bank data.
     */
    virtual void execute(Player& player, logic::Game& game) = 0;

    /**
     * @brief Stable category tag for logging and UI filtering.
     * @return A short type label such as `"Chance"` or `"Skill"`.
     */
    virtual std::string getCardType() const = 0;

    /**
     * @brief Expose the configured flavour text.
     * @return The description string passed to the constructor.
     */
    std::string getDescription() const;

protected:
    std::string description_;
};

}
