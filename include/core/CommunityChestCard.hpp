#pragma once

#include <functional>
#include <memory>
#include <string>

#include <core/ActionCard.hpp>
#include <core/Player.hpp>

#include <logic/Game.hpp>

//TODO: this class uses out-of-spec methods.

namespace core {

/**
 * @brief Community Chest deck entry mirroring `ChanceCard` mechanics.
 */
class CommunityChestCard : public ActionCard {
public:
    /**
     * @brief Build a card with a custom functor effect.
     * @param description Text printed when the card is revealed.
     * @param effect Functor that mutates `player` and `game`.
     * @note Extension (not in spec); the spec only lists `execute` / `getCardType`.
     */
    CommunityChestCard(std::string description, std::function<void(Player&, logic::Game&)> effect);

    void execute(Player& player, logic::Game& game) override;

    std::string getCardType() const override;

    /**
     * @brief Factory for doctor / tax style bank payments.
     * @param amount Funds removed from the player and credited to the bank.
     * @param description Card flavour text.
     * @note Extension (not in spec).
     */
    static std::unique_ptr<ActionCard> makePayBank(int amount, std::string description);

    /**
     * @brief Factory for grants paid from the bank to the player.
     * @param amount Funds added to the player and debited from the bank pool.
     * @param description Card flavour text.
     * @note Extension (not in spec).
     */
    static std::unique_ptr<ActionCard> makeCollect(int amount, std::string description);

private:
    std::function<void(Player&, logic::Game&)> effect_;  // Extension (not in spec).
};

}
