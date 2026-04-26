#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include "logic/InputMediator.hpp"
#include "ui/component/Widgets.hpp"

namespace core {
class Player;
class Property;
class ActionCard;
}  // namespace core

namespace ui {

/**
 * @brief Concrete `logic::InputMediator` implementation that renders blocking
 *        modal dialogs using the existing Widget hierarchy.
 *
 * Each public method runs its own inner SFML event loop until the player
 * dismisses the dialog, then returns the collected result to the game logic.
 *
 * Owned **by value** in `GameWindow`. Passed as a raw pointer to
 * `Game::setMediator(&dialogManager_)`.
 */
class DialogManager : public logic::InputMediator {
 public:
  /**
   * @param window The main render window. Stored by reference; must outlive
   *               this object. Used by the inner event loop and redraw cycle.
   */
  explicit DialogManager(sf::RenderWindow& window);
  ~DialogManager() override = default;

  // ── logic::InputMediator overrides ─────────────────────────────────────

  /**
   * @brief Ask the current player to choose one of their properties to receive
   *        the festival bonus.
   * @return Pointer to the chosen property, or nullptr if the player cancels.
   */
  core::Property* selectFestivalProperty(core::Player& player) override;

  /**
   * @brief Show a property deed and ask whether to buy or auction it.
   * @return `true` to buy; `false` to trigger auction.
   */
  bool offerProperty(core::Player& player, core::Property& property) override;

  /**
   * @brief Show both tax options and let the player choose.
   * @return `true` to use the percentage amount; `false` for the flat rate.
   */
  bool chooseTaxMethod(core::Player& player, int flatAmount,
                       int percentageAmount) override;

  /**
   * @brief Run a full blocking auction loop until only one participant
   *        remains or all pass.
   * @return {winner, finalBid}. winner is nullptr when all players pass.
   */
  std::pair<core::Player*, int> runAuction(
      core::Property* property,
      const std::vector<core::Player*>& eligiblePlayers) override;

  /**
   * @brief Show the player's hand and ask them to drop one card.
   * @return Raw pointer to the card to drop (non-owning).
   */
  core::ActionCard* selectCardToDrop(core::Player& player) override;

  // ── Informational dialogs (no logic return value) ──────────────────────

  /** @brief Show the full property deed card for @p prop. */
  void showDeed(const core::Property& prop);

  /** @brief Show all properties owned by @p player. */
  void showOwnedProperties(const core::Player& player);

  /** @brief Announce that @p player has gone bankrupt. */
  void showBankruptcy(const core::Player& player);

  /** @brief Show the result banner at the end of an auction. */
  void showAuctionResult(const core::Player* winner, int amount);

  // ── ActionPanel input dialogs ───────────────────────────────────────────

  /**
   * @brief Show two [−] N [+] spinners and return the chosen {d1, d2} pair.
   *        Each value is clamped to [1, 6].
   */
  std::pair<int, int> promptDiceOverride();

  /**
   * @brief Show the player's hand and return the index of the chosen card.
   * @return Index in [0, hand size). Returns 0 if hand is empty.
   */
  int promptCardChoice(const core::Player& player);

 private:
  sf::RenderWindow& window_;
  sf::Font font_;

  // ── Internal helpers ────────────────────────────────────────────────────

  /**
   * @brief Spin the SFML event loop, passing events to @p overlay and
   *        redrawing the window each frame, until @p isDone returns true or
   *        the window closes.
   */
  void runModalLoop(Panel& overlay, const std::function<bool()>& isDone);

  /**
   * @brief Build a centred modal panel of @p size using the standard
   *        Monopoly-themed backdrop (dark-brown panel, dim overlay).
   */
  std::unique_ptr<Panel> buildModalOverlay(sf::Vector2f size);

  /**
   * @brief Add a standard title band with @p title text to @p panel.
   */
  void addTitleBand(Panel& panel, const std::string& title, float width);

    /**
     * @brief Add a shared top-right CLOSE button on the title area.
     *
     * @param panel Target dialog panel.
     * @param enabled When false, the button is visible but disabled.
     * @param done Pointer toggled to true when CLOSE is clicked.
     */
    void addCloseButton(Panel& panel, bool enabled, bool* done);

  /**
   * @brief Build a single property deed card panel and add it to @p parent.
   *
   * @param parent  Target container.
   * @param prop    Property to describe.
   * @param origin  Top-left position of the deed within @p parent.
   * @param size    Bounding box of the deed.
   */
  void buildDeedPanel(Panel& parent, const core::Property& prop,
                      sf::Vector2f origin, sf::Vector2f size);
};

}  // namespace ui
