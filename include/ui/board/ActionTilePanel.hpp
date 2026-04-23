#pragma once

#include <memory>

#include "ui/board/TilePanel.hpp"
#include "ui/component/Widgets.hpp"

namespace ui {

class ActionTilePanel : public TilePanel {
 public:
  /**
   * @brief Construct a new Action Tile Panel object
   *
   * @param position
   * @param size
   * @param numTiles
   * @param orientation If set to Left or Right, the tile will be rotated 90
   * degrees clockwise. Default is Bottom.
   * @param iconPath
   * @param topText
   * @param bottomText
   */
  ActionTilePanel(sf::Vector2f position, int numTiles,
                  Orientation orientation = Orientation::Bottom,
                  const std::string& iconPath = "assets/icons/train.png",
                  const std::string& topText = "PETAK AKSI",
                  const std::string& bottomText = "$ 100");

  ~ActionTilePanel() override = default;

  void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window) override;

 private:
  void setupBottom();
  void setupLeft();
  void setupTop();
  void setupRight();

  std::string topText_;
  std::string bottomText_;
  std::string iconPath_;

  std::unique_ptr<Label> topLabel_;
  std::unique_ptr<ImagePanel> iconPanel_;
  std::unique_ptr<Label> bottomLabel_;

  static sf::Vector2f computePanelSize(int numTiles, Orientation orientation);
};

}  // namespace ui