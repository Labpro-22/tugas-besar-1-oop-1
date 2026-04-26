#pragma once

#include <string>

namespace core {

class Player;
class GameContext;

/**
 * @brief Abstract root for every drawable card (Chance, Community Chest,
 * Skill).
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
  explicit ActionCard(std::string description)
      : description_(std::move(description)) {}

  ActionCard(const ActionCard&) = delete;
  ActionCard& operator=(const ActionCard&) = delete;
  ActionCard(ActionCard&&) = delete;
  ActionCard& operator=(ActionCard&&) = delete;

  virtual ~ActionCard() = default;

  /**
   * @brief Apply this card's effect to @p player within @p context.
   * @param player The subject of the effect (drawer or activator).
   * @param context Live game facade (`logic::Game`) used to mutate board/bank
   * data.
   */
  virtual void execute(Player& player, GameContext& context) = 0;

  /**
   * @brief Stable category tag for logging and UI filtering.
   * @return A short type label such as `"Chance"` or `"Skill"`.
   */
  virtual std::string getCardType() const = 0;

  /**
   * @brief Expose the configured flavour text.
   * @return The description string passed to the constructor.
   */
  std::string getDescription() const { return description_; }

 protected:
  std::string description_;
};

/**
 * @brief Intermediate abstract type for skill cards held in the player's hand.
 *
 * Concrete subclasses implement the colourful effects described in the
 * specification while sharing construction rules with `ActionCard`.
 */
class SkillCard : public ActionCard {
 protected:
  /**
   * @brief Forward the flavour text to `ActionCard`.
   * @param description Human-readable card description.
   */
  explicit SkillCard(std::string description)
      : ActionCard(std::move(description)) {}
};

}  // namespace core
