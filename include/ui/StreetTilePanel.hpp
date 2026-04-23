#pragma once

#include "ui/Color.hpp"
#include "ui/TilePanel.hpp"

namespace ui {

class StreetTilePanel : public TilePanel {
 public:
  /**
   * @brief Construct a new Street Tile Panel object
   *
   * @param position
   * @param size
   * @param numTiles
   * @param orientation If set to Left or Right, the tile will be rotated 90
   * degrees clockwise. Default is Bottom.
   * @param accentColor
   * @param topText
   * @param bottomText
   */
  StreetTilePanel(sf::Vector2f position, int numTiles,
                  Orientation orientation = Orientation::Bottom,
                  sf::Color accentColor = board::base,
                  const std::string& topText = "PETAK JALAN",
                  const std::string& bottomText = "$ 100");

  ~StreetTilePanel() override = default;

  void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window) override;

 private:
  /**
   * @brief Return the tile accent height for street tile relative to the
   * current tile size
   *
   * @return float
   */
  float tileAccentHeight() const;

  void setupBottom();
  void setupLeft();
  void setupTop();
  void setupRight();

  sf::Color accentColor_;
  std::string topText_;
  std::string bottomText_;

  std::unique_ptr<Panel> accentPanel_;
  std::unique_ptr<Label> topLabel_;
  std::unique_ptr<Label> bottomLabel_;

  static sf::Vector2f computePanelSize(int numTiles, Orientation orientation);
};
}  // namespace ui