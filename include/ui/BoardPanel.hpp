#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Sensor.hpp>
#include <memory>
#include <string>

#include "ui/Color.hpp"
#include "ui/Widgets.hpp"

namespace ui {

class BoardPanel : public Panel {
 public:
  explicit BoardPanel(int tileNum = 9);
  ~BoardPanel() override = default;

  void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window) override;

 private:
  void setup();
  static constexpr int minTilesPerSide = 4;
  static constexpr int maxTilesPerSide = 14;
  int tileNum_ = 9; /**< Number of tile for each side excluding coner [4..14]
                      for 20x20 up to 60x60 board. Default is 9 (40x40) */
};

enum class Orientation { Bottom, Left, Top, Right };

class StreetTilePanel : public Panel {
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
  StreetTilePanel(sf::Vector2f position, sf::Vector2f size, int numTiles,
                  Orientation orientation = Orientation::Bottom,
                  sf::Color accentColor = board::base,
                  const std::string& topText = "PETAK JALAN",
                  const std::string& bottomText = "$ 100");

  ~StreetTilePanel() override = default;
  void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window) override;

 private:
  void setupBottom();
  void setupLeft();
  void setupTop();
  void setupRight();

  int numTiles_;
  Orientation orientation_;
  sf::Color accentColor_;
  std::string topText_;
  std::string bottomText_;
  sf::Font font_;

  std::unique_ptr<Panel> accentPanel_;
  std::unique_ptr<Label> topLabel_;
  std::unique_ptr<Label> bottomLabel_;
};

class ActionTilePanel : public Panel {};

class CornerTilePanel : public Panel {};

}  // namespace ui
