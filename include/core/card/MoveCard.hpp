#pragma once

#include <core/card/Card.hpp>
#include <core/player/Player.hpp>
#include <memory>
#include <string>

namespace core {

class GameContext;

/**
 * @brief Skill that advances the current player a relative number of spaces.
 */
class MoveCard : public SkillCard {
 public:
  /**
   * @brief Capture the relative movement amount.
   * @param steps Number of clockwise spaces to travel.
   * @param description Human-readable description for UI logs.
   */
  MoveCard(int steps, std::string description);

  void execute(Player& player, GameContext& context) override;

  std::string getCardType() const override;

  /**
   * @brief Factory helper for deck loaders.
   * @param steps Relative movement (wrapped using `GameContext::getBoardSize`).
   * @param description Human-readable description for UI logs.
   * @note Extension (not in spec).
   */
  static std::unique_ptr<ActionCard> make(int steps, std::string description);
  // IDK why I made this :D

 private:
  int steps_;
};

}  // namespace core
