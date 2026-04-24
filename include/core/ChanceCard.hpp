#pragma once

#include <functional>
#include <memory>
#include <string>

#include <core/ActionCard.hpp>
#include <core/Player.hpp>

//TODO: this class uses out-of-spec methods.

namespace core {

class GameContext;

/**
 * @brief Chance deck entry implemented as description + functor payload.
 */
class ChanceCard : public ActionCard {
public:
    /**
     * @brief Build a card with a custom functor effect.
     * @param description Text printed when the card is revealed.
     * @param effect Functor that mutates `player` and `context`.
     * @note Extension (not in spec); the spec only lists `execute` / `getCardType`.
     */
    ChanceCard(std::string description, std::function<void(Player&, GameContext&)> effect);

    void execute(Player& player, GameContext& context) override;

    std::string getCardType() const override;

    /**
     * @brief Factory for "move back N spaces" style instructions.
     * @param spaces Number of spaces to move backward (wrapped on the board).
     * @param description Card flavour text.
     * @note Extension (not in spec).
     */
    static std::unique_ptr<ActionCard> makeMoveBack(int spaces, std::string description);

    /**
     * @brief Factory for cash penalties paid to the bank.
     * @param amount Funds removed from the player and credited to the bank.
     * @param description Card flavour text.
     * @note Extension (not in spec).
     */
    static std::unique_ptr<ActionCard> makePayBank(int amount, std::string description);

    /**
     * @brief Factory that teleports the current player to jail.
     * @param description Card flavour text.
     * @note Extension (not in spec).
     */
    static std::unique_ptr<ActionCard> makeGoToJail(std::string description);

    /**
     * @brief Factory that jumps the player to an absolute board index.
     * @param index Zero-based tile index resolved by teammates.
     * @param description Card flavour text.
     * @note Extension (not in spec).
     */
    static std::unique_ptr<ActionCard> makeAdvanceTo(int index, std::string description);

private:
    std::function<void(Player&, GameContext&)> effect_;  // Extension (not in spec).
};

}
