#pragma once

#include <SFML/Graphics/Font.hpp>

#include "ui/AssetsManager.hpp"
#include "ui/board/TileGeometry.hpp"
#include "ui/component/Color.hpp"
#include "ui/component/Constants.hpp"
#include "ui/component/Style.hpp"
#include "ui/component/Widgets.hpp"

namespace ui {

enum class Orientation { Bottom, Left, Top, Right };

class TilePanel : public Panel {
 public:
  TilePanel(sf::Vector2f position, sf::Vector2f size, int numTiles,
            Orientation orientation)
      : Panel(position, size,
              PanelStyle(board::base, palette::black, size::lineThickness)),
        orientation_(orientation),
        tileGeo_(numTiles),
        font_(AssetsManager::get().getFont(font::primaryFamily)) {}

  ~TilePanel() override = default;

 protected:
  Orientation orientation_;
  TileGeometry tileGeo_;
  sf::Font font_;
};

}  // namespace ui