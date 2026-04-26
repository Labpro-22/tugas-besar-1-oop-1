#pragma once

#include <memory>
#include <string>
#include <vector>

namespace logic {

class Game;

}

namespace core {

class ActionCard;
class Property;

enum class Avatar {
  COPILOT,
  CLAUDE,
  GEMINI,
  CHATGPT,
};

/**
 * @brief Abstract player controller shared by humans and COM opponents.
 *
 * Owns liquid cash, board index, jail state, and observer lists for properties
 * and cards. Property objects are owned elsewhere; `ownedProperties_` is a
 * non-owning cache. Action cards are owned by decks; `heldCards_` stores loans.
 */
class Player {
 public:
  /**
   * @brief Construct a player with display metadata.
   * @param name Display name for logs and save files.
   * @param avatar Player avatar identity used by UI and game metadata.
   */
  Player(std::string name, Avatar avatar);

  Player(const Player&) = delete;
  Player& operator=(const Player&) = delete;
  Player(Player&&) = delete;
  Player& operator=(Player&&) = delete;

  virtual ~Player() = default;

  /**
   * @brief Drive one full turn for this controller.
   * @param game Singleton game instance coordinating state transitions.
   */
  virtual void takeTurn(logic::Game& game) = 0;

  /**
   * @brief Report whether this controller represents a human UI client.
   * @return `true` for `HumanPlayer`, otherwise `false`.
   */
  virtual bool isHuman() = 0;

  /**
   * @brief Aggregate wealth used for rankings and tax calculations.
   * @return Cash on hand plus mortgaged mortgage value or list price for each
   * owned lot.
   */
  int getNetWorth() const noexcept;

  /**
   * @brief Predicate for payment feasibility checks.
   * @param amount Non-negative cash requirement.
   * @return `true` when `balance >= amount`.
   */
  bool canAfford(int amount) const noexcept;

  /**
   * @brief Mark the player as incarcerated without adjusting board index.
   */
  void goToJail();

  /**
   * @brief Clear jail state and reset the failed-roll counter.
   */
  void releaseFromJail();

  /**
   * @brief Register a newly acquired property pointer.
   * @param property Non-owning observer; must outlive the player.
   */
  void addProperty(Property& property);

  /**
   * @brief Drop a property from the local cache (ownership unchanged).
   * @param property Reference previously passed to `addProperty`.
   */
  void removeProperty(Property& property);

  /**
   * @brief Credit cash from the bank or another player.
   * @param money Positive amount to add.
   * @return Reference to this player for chaining.
   */
  Player& operator+=(int money);

  /**
   * @brief Debit cash toward the bank or another player.
   * @param money Positive amount to subtract.
   * @return Reference to this player for chaining.
   */
  Player& operator-=(int money);

  /**
   * @brief Compare net worth against another player.
   * @param other Opponent reference.
   * @return `true` when this player's net worth is strictly lower.
   */
  bool operator<(const Player& other) const noexcept;

  /**
   * @brief Compare net worth against another player.
   * @param other Opponent reference.
   * @return `true` when this player's net worth is strictly higher.
   */
  bool operator>(const Player& other) const noexcept;

  /**
   * @brief Equality check on total wealth.
   * @param other Opponent reference.
   * @return `true` when both net worth totals match.
   */
  bool operator==(const Player& other) const noexcept;

  /**
   * @brief Display name accessor for serializers and `TransactionLogger`.
   * @return The configured player name.
   * @note Extension (not in spec).
   */
  const std::string& getName() const noexcept;

  /**
   * @brief Avatar accessor for UI rendering and identification.
   * @return The configured avatar enum.
   */
  Avatar getAvatar() const noexcept;

  /**
   * @brief Liquid cash accessor.
   * @return Current balance in Monopoly money units.
   * @note Extension (not in spec).
   */
  int getBalance() const noexcept;

  /**
   * @brief Board index accessor.
   * @return Zero-based tile index on the active board.
   * @note Extension (not in spec).
   */
  int getPosition() const noexcept;

  /**
   * @brief Teleport helper used by cards and jail transitions.
   * @param index Absolute board index after wrapping by teammates.
   * @note Extension (not in spec).
   */
  void setPosition(int index) noexcept;

  /**
   * @brief Jail predicate.
   * @return `true` while the player must follow jail rules.
   * @note Extension (not in spec).
   */
  bool isInJail() const noexcept;

  /**
   * @brief Bankruptcy predicate.
   * @return `true` when the player has been eliminated.
   * @note Extension (not in spec).
   */
  bool isBankrupted() const noexcept;

  /**
   * @brief Mark this player as bankrupt and eliminated.
   */
  void declareBankrupt() noexcept;

  /**
    * @brief Attempt to add an owned action card to the hand.
    * @param card Owned pointer transferred into the player hand.
   * @throws InvalidMoveException When the hand already holds three cards.
   * @note Extension (not in spec).
   */
    void addCard(std::unique_ptr<ActionCard> card);

  /**
    * @brief Remove a card from the hand and transfer ownership to the caller.
   * @param card Pointer previously inserted via `addCard`.
    * @return Owned pointer when found; otherwise `nullptr`.
   */
    std::unique_ptr<ActionCard> removeCard(ActionCard* card);

  /**
   * @brief Activate shield protection until the next `resetPerTurnFlags`.
   * @note Extension (not in spec).
   */
  void useShield();

  /**
   * @brief Query whether shield mitigation should apply.
   * @return `true` when a `ShieldCard` is active for this turn cycle.
   * @note Extension (not in spec).
   */
  bool isShielded() const noexcept;

  /**
   * @brief Record that a skill card was activated this turn.
   * @throws InvalidMoveException When a second skill tries to fire in the same
   * turn.
   * @note Extension (not in spec).
   */
  void consumeSkillUse();

  /**
   * @brief Clear per-turn UI/COM flags at the start of a new turn.
   * @note Extension (not in spec).
   */
  void resetPerTurnFlags() noexcept;

  /**
   * @brief Store a discount multiplier granted by `DiscountCard`.
   * @param rate Fractional discount between `0` and `1`.
   * @note Extension (not in spec).
   */
  void applyDiscount(float rate) noexcept;

  /**
   * @brief Read the active discount multiplier for the next property purchase.
   * @return Fractional discount between `0` and `1`.
   */
  float getDiscountRate() const noexcept;

  /**
   * @brief Hook for cards that need a discrete UI choice.
   * @param context Developer-readable reason string for logging.
   * @param defaultIndex Fallback index when automation declines to choose.
   * @param optionCount Exclusive upper bound on valid indices.
   * @return The selected index in `[0, optionCount)`.
   * @note Extension (not in spec).
   */
  virtual int promptChoice(const std::string& context, int defaultIndex,
                           int optionCount);

  /**
   * @brief Expose owned properties for demolition/build heuristics.
   * @return Read-only view of the cached `Property*` list.
   * @note Extension (not in spec).
   */
  const std::vector<Property*>& getOwnedProperties() const noexcept;

  /**
   * @brief Expose held cards for UI deck rendering.
    * @return Snapshot of the currently held raw pointers.
   * @note Extension (not in spec).
   */
    std::vector<ActionCard*> getHeldCards() const;

  /**
   * @brief Failed jail-roll counter accessor.
   * @return Number of unsuccessful exit attempts this incarceration.
   * @note Extension (not in spec).
   */
  int getJailTurns() const noexcept;

  /**
   * @brief Increment jail attempts after a failed doubles roll.
   * @note Extension (not in spec).
   */
  void incrementJailTurns() noexcept;

  /**
   * @brief Reset jail attempts after paying bail or using a jail card.
   * @note Extension (not in spec).
   */
  void clearJailTurns() noexcept;

  /**
   * @brief Set balance directly to restore data from saved files.
   * Do not use for normal transactions, use operator+= / operator-= instead.
   * Only can be called by GameLoader.
   */
  void setBalance(int amount) noexcept;

 protected:
  std::string name_;
  Avatar avatar_;
  int balance_;
  int position_;
  bool inJail_;
  bool isBankrupt_;
  int jailTurns_;
  std::vector<Property*> ownedProperties_;
  std::vector<std::unique_ptr<ActionCard>> heldCards_;
  bool shieldActive_;       // Extension (not in spec).
  bool usedSkillThisTurn_;  // Extension (not in spec).
  float discountRate_;      // Extension (not in spec).
};

}  // namespace core
