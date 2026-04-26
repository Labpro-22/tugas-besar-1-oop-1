#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Sensor.hpp>
#include <functional>
#include <unordered_map>
#include <vector>

#include "ui/board/TilePanel.hpp"
#include "ui/component/Widgets.hpp"

namespace logic {
class Board;
}

namespace core {
class Player;
enum class Avatar : int;
}

namespace ui {

class BoardPanel : public Panel {
 public:
  explicit BoardPanel(int tileNum = 9);
  ~BoardPanel() override = default;

  void populateTiles(const logic::Board& board);
  void setOnTileSelected(std::function<void(const TileInfo&)> onTileSelected);
  void setPlayers(const std::vector<core::Player*>& players);

  void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window) override;

 private:
  void setup();
  static constexpr int minTilesPerSide = 4;
  static constexpr int maxTilesPerSide = 14;
  int tileNum_ = 9; /**< Number of tile for each side excluding coner [4..14]
                      for 20x20 up to 60x60 board. Default is 9 (40x40) */
  std::function<void(const TileInfo&)> onTileSelected_;
  std::vector<core::Player*> players_;
  std::unordered_map<core::Avatar, sf::Texture*> avatarTextures_;
};

}  // namespace ui
