#pragma once

#include <core/player/Player.hpp>

namespace core {

/**
 * @brief Player implementation that blocks on UI events instead of automating.
 */
class HumanPlayer : public Player {
 public:
  /**
   * @brief Construct a human-controlled seat.
   * @param name Display name persisted in saves/logs.
   * @param avatar Avatar identity for this player.
   */
  HumanPlayer(std::string name, Avatar avatar)
      : Player(std::move(name), avatar) {}

  ~HumanPlayer() override = default;

  void takeTurn(PlayerTurnContext& turnContext) override {
    resetPerTurnFlags();
    (void)turnContext;
    // UI layer is responsible for issuing commands; this stub simply yields.
  }

  bool isHuman() override { return true; }
};

}  // namespace core
