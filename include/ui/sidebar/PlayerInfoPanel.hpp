#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "core/player/Player.hpp"
#include "ui/component/Widgets.hpp"
#include "ui/sidebar/PlayerCard.hpp"

namespace logic {
class Board;
}

namespace ui {

class PlayerInfoPanel : public Panel {
 public:
  PlayerInfoPanel();
  ~PlayerInfoPanel() override;

  void setPlayers(const std::vector<core::Player*>& players,
                  const logic::Board& board, core::Player* currentPlayer);
  void setOnPlayerSelected(std::function<void(core::Player&)> onPlayerSelected);

  void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window) override;

 private:
  std::vector<std::unique_ptr<PlayerCard>> playerCards_;
  std::function<void(core::Player&)> onPlayerSelected_;
};

}  // namespace ui
