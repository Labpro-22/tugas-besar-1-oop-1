#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <memory>
#include <string>
#include <vector>

#include "ui/board/TilePanel.hpp"
#include "ui/component/Widgets.hpp"

namespace ui {

class DialogManager;
}

namespace logic {
class Game;
}

namespace ui {

class ActionPanel : public Panel {
 public:
  ActionPanel();
  ~ActionPanel() override = default;

  void setGameContext(logic::Game* game, DialogManager* dialogManager);

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
  void setupStatusRows(float width, float& currentY);

  void setupNoneSelected();
  void setupSelectedExist(const TileInfo& tileInfo);
  void refreshStatusRows();
  void refreshLogLines();
  void clampLogScroll();
  bool isInRect(const sf::Vector2f& point, const sf::FloatRect& rect) const;

  sf::Font font_;
  Label* turnValueLabel_ = nullptr;
  Label* diceValueLabel_ = nullptr;

  float selectedTilePanelWidth_ = 0.0f;
  float selectedTilePanelY_ = 0.0f;
  std::unique_ptr<Panel> selectedTilePanel_ = nullptr;
  std::unique_ptr<Button> buildButton_;
  std::unique_ptr<Button> sellButton_;
  std::unique_ptr<Button> mortgageButton_;
  std::unique_ptr<Button> unmortgageButton_;
  logic::Game* game_ = nullptr;
  DialogManager* dialogManager_ = nullptr;

  sf::FloatRect logBodyRect_{};
  sf::FloatRect logTrackRect_{};
  sf::FloatRect logThumbRect_{};
  std::vector<std::string> logLines_;
  int logFirstVisibleLine_ = 0;
  int logVisibleLineCount_ = 1;
  float logLineHeight_ = 16.0f;
  bool draggingLogThumb_ = false;
  float logDragOffsetY_ = 0.0f;
};

}  // namespace ui
