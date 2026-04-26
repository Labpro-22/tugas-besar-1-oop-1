#pragma once

#include <core/Property.hpp>
#include <string>

namespace core {

class Player;

/**
 * @brief Strategy interface consulted by `COMPlayer` for economic decisions.
 *
 * Implementations stay stateless; all context arrives through method arguments.
 */
class COMStrategy {
 public:
  virtual ~COMStrategy() = default;

  /**
   * @brief Decide whether the COM should purchase an offered property.
   * @param property Lot under consideration (never `nullptr` in real flows).
   * @param player Acting COM instance.
   * @return `true` when the purchase should be attempted.
   */
  virtual bool shouldBuy(Property* property, Player& player) const noexcept =
      0;

  /**
   * @brief Produce a maximum bid increment for auction resolution.
   * @param property Lot being auctioned.
   * @param player Acting COM instance.
   * @param previous_bid Last acknowledged bid from another participant.
   * @return Non-negative bid cap for this COM.
   */
  virtual int bidAmount(Property* property, Player& player,
                        int previous_bid) const noexcept = 0;

  /**
   * @brief Decide whether to pay for another building on a color group.
   * @param property Street-like lot that would receive the build.
   * @param player Acting COM instance.
   * @return `true` when `Game::buildHouse` should run.
   */
  virtual bool shouldBuild(Property* property,
                           Player& player) const noexcept = 0;

  /**
   * @brief Decide whether mortgaging a lot improves survivability.
   * @param property Candidate lot to mortgage.
   * @param player Acting COM instance.
   * @return `true` when `Game::mortgageProperty` should run.
   */
  virtual bool shouldMortgage(Property* property,
                              Player& player) const noexcept = 0;

  /**
   * @brief Choose an index for scripted card effects that need UI input.
   * @param context Human-readable reason for analytics/logging.
   * @param defaultIndex Safe fallback when automation declines.
   * @param optionCount Number of discrete choices available to the player.
   * @return A clamped index in `[0, optionCount)`; when `optionCount <= 0`,
   *         returns `defaultIndex`.
   * @note Extension (not in spec).
   */
  virtual int suggestChoice(const std::string&, int defaultIndex,
                            int) const noexcept {
    return defaultIndex;
  }
};

}  // namespace core
