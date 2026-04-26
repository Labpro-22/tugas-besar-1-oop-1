#pragma once

#include <utility>
#include <vector>

namespace core {
class Player;
class Property;
class ActionCard;
}  // namespace core

namespace logic {

/**
 * @brief Abstract interface through which the game logic requests blocking UI
 *        decisions. The single concrete implementation is `ui::DialogManager`.
 */
class InputMediator {
 public:
  virtual ~InputMediator() = default;

  virtual core::Property* selectFestivalProperty(core::Player& player) = 0;

  /**
   * @return `true` when the player accepts the purchase; `false` to trigger
   *         auction instead.
   */
  virtual bool offerProperty(core::Player& player,
                             core::Property& property) = 0;

  virtual bool chooseTaxMethod(core::Player& player, int flatAmount,
                               int percentageAmount) = 0;

  /**
   * @return {winner, finalBid}. winner is nullptr when all players pass.
   */
  virtual std::pair<core::Player*, int> runAuction(
      core::Property* property,
      const std::vector<core::Player*>& eligiblePlayers) = 0;

  virtual core::ActionCard* selectCardToDrop(core::Player& player) = 0;
};

}  // namespace logic