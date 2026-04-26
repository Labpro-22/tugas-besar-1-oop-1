#pragma once

#include <core/player/COMStrategy.hpp>

namespace core {

/**
 * @brief Conservative COM profile that hoards cash and mortgages early.
 */
class SimpleCOMStrategy : public COMStrategy {
 public:
  /**
   * @brief Approve purchases only when the list price is <= 40% of cash.
   */
  bool shouldBuy(Property* property, Player& player,
                 logic::Game& game) const noexcept override;

  /**
   * @brief Cap bids at ~110% of list price without exceeding the cash stack.
   */
  int bidAmount(Property* property, Player& player, logic::Game& game,
                int previous_bid) const noexcept override;

  /**
   * @brief Build only when the player keeps >3x the next house cost in cash.
   */
  bool shouldBuild(Property* property, Player& player,
                   logic::Game& game) const noexcept override;

  /**
   * @brief Mortgage aggressively whenever liquidity drops below `200`.
   */
  bool shouldMortgage(Property* property, Player& player,
                      logic::Game& game) const noexcept override;
};

}  // namespace core