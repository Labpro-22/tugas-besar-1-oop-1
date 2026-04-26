#pragma once

#include <SFML/Graphics/Font.hpp>
#include <functional>
#include <memory>
#include <string>

#include "ui/board/TileGeometry.hpp"
#include "ui/component/Color.hpp"
#include "ui/component/Widgets.hpp"

namespace ui {

enum class Orientation { Bottom, Left, Top, Right };

class TileInfo {
 public:
  int tileIndex = -1;
  std::string title;
  std::string iconPath;
  bool isProperty = false;
  bool useAccent = false;
  sf::Color accentColor = board::base;
  bool actionsEnabled = true;
};

class TilePanel : public Panel {
 public:
  TilePanel(sf::Vector2f position, sf::Vector2f panelSize, int numTiles,
            Orientation orientation);

  ~TilePanel() override = default;

  void setOnSelected(std::function<void(const TileInfo&)> onSelected);
  const TileInfo& selectionInfo() const { return selectionInfo_; }
  TileInfo& mutableSelectionInfo() { return selectionInfo_; }

  void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window) override;

 protected:
  static sf::Vector2f computeSidePanelSize(int numTiles,
                                           Orientation orientation);

  Orientation orientation_;
  TileGeometry tileGeo_;
  sf::Font font_;
  TileInfo selectionInfo_;

 private:
  std::function<void(const TileInfo&)> onSelected_;
  std::unique_ptr<Button> interactionButton_;
};

}  // namespace ui