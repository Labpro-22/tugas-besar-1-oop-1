#pragma once

#include <memory>

#include "ui/board/TilePanel.hpp"
#include "ui/component/Widgets.hpp"

namespace ui {

class ActionPanel : public Panel {
 public:
  ActionPanel();
  ~ActionPanel() override = default;

  void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window) override;

  void setSelectedTile(const TileInfo& tileInfo);

 private:
  void setup();
  void setupGameActionPanel(float width, float& currentY);
  void setupTileOptionPanel(float width, float& currentY);
  void setupLogPanel(float width, float& currentY);
  void setupGameOptionPanel(float width, float& currentY);

  void setupNoneSelected();
  void setupSelectedExist(const TileInfo& tileInfo);

  sf::Font font_;
  float selectedTilePanelWidth_ = 0.0f;
  float selectedTilePanelY_ = 0.0f;
  std::unique_ptr<Panel> selectedTilePanel_ = nullptr;
  std::unique_ptr<Button> buildButton_;
  std::unique_ptr<Button> sellButton_;
  std::unique_ptr<Button> mortgageButton_;
  std::unique_ptr<Button> unmortgageButton_;
};

}  // namespace ui
