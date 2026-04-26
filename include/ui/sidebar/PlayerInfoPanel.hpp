#pragma once

#include <memory>
#include <vector>

#include "ui/component/Widgets.hpp"
#include "ui/sidebar/Game_temp.hpp"
#include "ui/sidebar/PlayerCard.hpp"

namespace ui {

class PlayerInfoPanel : public Panel {
 public:
  PlayerInfoPanel();
  ~PlayerInfoPanel() override;

  void setPlayers(const std::vector<const temp::Player*>& players);

  void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window) override;

 private:
  std::vector<std::unique_ptr<PlayerCard>> playerCards_;
};

}  // namespace ui
