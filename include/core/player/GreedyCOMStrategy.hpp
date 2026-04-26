#pragma once

#include <core/player/COMStrategy.hpp>
#include <string>

namespace core {

/**
 * @brief Aggressive COM profile that spends cash liberally.
 */
class GreedyCOMStrategy : public COMStrategy {
 public:
  /**
   * @brief Purchase whenever the player can liquidly afford the sticker price.
   */
  bool shouldBuy(Property* property, Player& player,
                 logic::Game& game) const noexcept override;

  /**
   * @brief Raise bids up to `min(balance, previous*1.25 + 50)`.
   */
  int bidAmount(Property* property, Player& player, logic::Game& game,
                int previous_bid) const noexcept override;

  /**
   * @brief Build whenever the next house cost is affordable.
   */
  bool shouldBuild(Property* property, Player& player,
                   logic::Game& game) const noexcept override;

  /**
   * @brief Mortgage only when the treasury is already empty/negative.
   */
  bool shouldMortgage(Property* property, Player& player,
                      logic::Game& game) const noexcept override;

  /**
   * @brief Prefer the highest-index option to mimic risky play styles.
   * @note Extension (not in spec); overrides `COMStrategy::suggestChoice`.
   */
  int suggestChoice(const std::string& context, int defaultIndex,
                    int optionCount) const noexcept override;
};

}  // namespace core
