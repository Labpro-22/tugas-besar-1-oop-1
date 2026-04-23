#include "ui/BoardPanel.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <stdexcept>
#include <utility>

#include "ui/ActionTilePanel.hpp"
#include "ui/Color.hpp"
#include "ui/Constants.hpp"
#include "ui/CornerTilePanel.hpp"
#include "ui/TileGeometry.hpp"
#include "ui/TilePanel.hpp"
#include "ui/Widgets.hpp"

namespace ui {

BoardPanel::BoardPanel(int tileNum)
    : Panel({size::sideWidth, 0}, {size::boardSize, size::boardSize},
            component::background),
      tileNum_(tileNum) {
  if (tileNum_ < minTilesPerSide || tileNum_ > maxTilesPerSide) {
    throw std::runtime_error("Invalid board configuration.");
  }
  auto boardPanel = std::make_unique<Panel>(position(), size(), board::base);
  addChild(std::move(boardPanel));
  setup();
}

void BoardPanel::setup() {
  const TileGeometry geo(tileNum_);
  const float cornerSize = geo.cornerSide();
  const float tileWidth = geo.sideWidth();

  // Top-left corner of each side's first tile.
  const auto leftPos = sf::Vector2f(position_.x, position_.y + cornerSize);
  const auto topPos = sf::Vector2f(position_.x + cornerSize, position_.y);
  const auto rightPos = sf::Vector2f(position_.x + size_.x - cornerSize,
                                     position_.y + cornerSize);
  const auto bottomPos = sf::Vector2f(position_.x + cornerSize,
                                      position_.y + size_.y - cornerSize);

  // DEBUG
  for (int i = 0; i < tileNum_; i++) {
    // Left
    addChild(std::make_unique<ActionTilePanel>(
        leftPos + sf::Vector2f(0, i * tileWidth), tileNum_, Orientation::Left,
        "assets/icons/train.png", "ABC", "$ 100"));

    // Top
    addChild(std::make_unique<ActionTilePanel>(
        topPos + sf::Vector2f(i * tileWidth, 0), tileNum_, Orientation::Top,
        "assets/icons/chance-pink.png", "PENDEK", "$ 100"));

    // Right
    addChild(std::make_unique<ActionTilePanel>(
        rightPos + sf::Vector2f(0, i * tileWidth), tileNum_, Orientation::Right,
        "assets/icons/go.png", "YOGYAKARTA", "$ 100"));

    // Bottom
    addChild(std::make_unique<ActionTilePanel>(
        bottomPos + sf::Vector2f(i * tileWidth, 0), tileNum_,
        Orientation::Bottom, "assets/icons/train-kai.png",
        "SANGATPANJANGTEKSNYAINI", "$ 100"));
  }

  addChild(std::make_unique<CornerTilePanel>(
      position_ + size_ - sf::Vector2f(cornerSize, cornerSize), tileNum_,
      CornerType::GO, Orientation::Bottom, "assets/icons/go.png", "MULAI",
      "DAPAT UANG"));
  addChild(std::make_unique<CornerTilePanel>(
      position_ + sf::Vector2f(0, size_.y - cornerSize), tileNum_,
      CornerType::JAIL, Orientation::Left, "assets/icons/jail.png", "HANYA",
      "BERKUNJUNG"));
  addChild(std::make_unique<CornerTilePanel>(
      position_, tileNum_, CornerType::FREE, Orientation::Top,
      "assets/icons/free-park.png", "PARKIR", "GRATIS"));
  addChild(std::make_unique<CornerTilePanel>(
      position_ + sf::Vector2f(size_.x - cornerSize, 0), tileNum_,
      CornerType::GO_TO_JAIL, Orientation::Right, "assets/icons/go-to-jail.png",
      "PERGI", "KE PENJARA"));
}

void BoardPanel::handleEvent(sf::Event& event, sf::RenderWindow& window) {
  Panel::handleEvent(event, window);
}

void BoardPanel::render(sf::RenderWindow& window) { Panel::render(window); }

void BoardPanel::update(sf::RenderWindow& window) { Panel::update(window); }

}  // namespace ui
