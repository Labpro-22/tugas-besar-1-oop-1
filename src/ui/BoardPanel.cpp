#include "ui/BoardPanel.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <stdexcept>
#include <utility>

#include "ui/Color.hpp"
#include "ui/Constants.hpp"
#include "ui/StreetTilePanel.hpp"
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
    addChild(std::make_unique<StreetTilePanel>(
        leftPos + sf::Vector2f(0, i * tileWidth), tileNum_, Orientation::Left,
        board::property::red, "YOGYAKARTA", "$ 100"));

    // Top
    addChild(std::make_unique<StreetTilePanel>(
        topPos + sf::Vector2f(i * tileWidth, 0), tileNum_, Orientation::Top,
        board::property::yellow, "YOGYAKARTA", "$ 100"));

    // Right
    addChild(std::make_unique<StreetTilePanel>(
        rightPos + sf::Vector2f(0, i * tileWidth), tileNum_, Orientation::Right,
        board::property::green, "YOGYAKARTA", "$ 100"));

    // Bottom
    addChild(std::make_unique<StreetTilePanel>(
        bottomPos + sf::Vector2f(i * tileWidth, 0), tileNum_,
        Orientation::Bottom, board::property::darkBlue,
        "SANGATPANJANGTEKSNYAINI", "$ 100"));
  }
}

void BoardPanel::handleEvent(sf::Event& event, sf::RenderWindow& window) {
  Panel::handleEvent(event, window);
}

void BoardPanel::render(sf::RenderWindow& window) { Panel::render(window); }

void BoardPanel::update(sf::RenderWindow& window) { Panel::update(window); }

}  // namespace ui
