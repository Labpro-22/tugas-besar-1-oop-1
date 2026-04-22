#pragma once

#include "ui/Widgets.hpp"

namespace ui {

class ActionPanel : public Panel {
 public:
  ActionPanel();
  ~ActionPanel() override;

  void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window) override;

 private:
  void setup();
  void setupGameActionPanel(float width, float& currentY);
  void setupTileOptionPanel(float width, float& currentY);
  void setupLogPanel(float width, float& currentY);
  void setupGameOptionPanel(float width, float& currentY);

  sf::Font font_;
};

}  // namespace ui
