#pragma once

#include <SFML/System/Vector2.hpp>
#include <memory>
#include <string>

#include "core/Tiles.hpp"
#include "ui/board/TilePanel.hpp"

namespace ui {

class CornerTilePanel : public TilePanel {
 public:
  CornerTilePanel(sf::Vector2f position, int numTiles,
                  core::SpecialTileType cornerType, Orientation orientation,
                  const std::string& iconPath = "assets/icons/train.png",
                  const std::string& topText = "PETAK",
                  const std::string& bottomText = "UJUNG");

  ~CornerTilePanel() override = default;

  void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window) override;

 private:
  void setupNonJail();
  void setupJail();

  core::SpecialTileType cornerType_;
  std::string topText_;
  std::string bottomText_;
  std::string iconPath_;

  std::unique_ptr<Label> topLabel_;
  std::unique_ptr<ImagePanel> iconPanel_;
  std::unique_ptr<Label> bottomLabel_;

  sf::Vector2f computeTopTextPos();
  sf::Vector2f computeTopTextSize() const;
  sf::Vector2f computeIconPos();
  sf::Vector2f computeIconSize() const;
  sf::Vector2f computeBottomTextPos();
  sf::Vector2f computeBottomTextSize() const;
};
}  // namespace ui