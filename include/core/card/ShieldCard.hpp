#pragma once

#include <core/Player.hpp>
#include <core/card/Card.hpp>
#include <memory>
#include <string>

namespace core {

class GameContext;

/**
 * @brief Skill that flags the player as immune to harmful payments briefly.
 */
class ShieldCard : public SkillCard {
 public:
  /**
   * @brief Construct with flavour text.
   * @param description Human-readable description for UI logs.
   */
  explicit ShieldCard(std::string description);

  void execute(Player& player, GameContext& context) override;

  std::string getCardType() const override;

  /**
   * @brief Factory helper for deck loaders.
   * @param description Human-readable description for UI logs.
   * @note Extension (not in spec).
   */
  static std::unique_ptr<ActionCard> make(std::string description);
  // same functionality as make in ChanceCard.cpp and makeRandom in
  // DiscountCard.cpp
};

}  // namespace core
