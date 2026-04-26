#pragma once

#include <core/card/Card.hpp>
#include <core/player/Player.hpp>
#include <memory>
#include <string>

namespace core {

class GameContext;

/**
 * @brief Skill that destroys exactly one building on a chosen opponent's
 *        Street property.
 *
 * @note Demolition removes a single house/hotel step per execution. A hotel
 *       counts as a single building step, matching `Street::demolish(1)`.
 *       repeated activations of multiple Demolition cards are required to
 *       fully clear a property. We deliberately keep one-step demolition to
 *       align with `Street::demolish` semantics owned by the property layer.
 */
class DemolitionCard : public SkillCard {
 public:
  /**
   * @brief Construct with flavour text.
   * @param description Human-readable description for UI logs.
   */
  explicit DemolitionCard(std::string description);

  void execute(Player& player, GameContext& context) override;

  std::string getCardType() const override;

  /**
   * @brief Factory helper for deck loaders.
   * @param description Human-readable description for UI logs.
   * @note Extension (not in spec).
   */
  static std::unique_ptr<ActionCard> make(std::string description);
  // same functionality as make in ChanceCard.cpp and ShieldCard.cpp, also
  // makeRandom in DiscountCard.cpp
};

}  // namespace core
