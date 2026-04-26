#pragma once

#include <memory>
#include <string>

#include <core/Player.hpp>
#include <core/SkillCard.hpp>

namespace core {

class GameContext;

/**
 * @brief Skill that rolls a random discount percentage when acquired.
 *
 * @note `remainingDuration_` exists only so `<SISA_DURASI>` in the save
 *       format (`Spesifikasi.md:576`) has a meaningful value to serialise.
 *       The active discount itself lives on `Player` and is cleared by
 *       `Player::resetPerTurnFlags()`.
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

    /**
     * @brief Reports the remaining number of turns this card is valid for.
     * @return Currently always `1` for newly minted cards.
     * @note Extension. needed so save/load can serialise the
     *       `<SISA_DURASI>` field documented in `Spesifikasi.md:576`.
     */
    int getRemainingDuration() const noexcept;

    /**
     * @brief Returns the rolled discount rate (0..1).
     * @note Extension. Needed so save/load can serialise the discount
     *       value the card was rolled with.
     */
    float getDiscountRate() const noexcept;

private:
    float discountRate_;
    int remainingDuration_ = 1;
};

}
