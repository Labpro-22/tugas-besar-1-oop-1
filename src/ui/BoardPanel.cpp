#include "ui/BoardPanel.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <stdexcept>
#include <utility>

#include "ui/Color.hpp"
#include "ui/Constants.hpp"
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
  const auto cornerSize = size::cornerTileSize(tileNum_);

  // Always top-left corner of the tile

  auto leftPos = sf::Vector2f(position_.x, position_.y + cornerSize);
  auto topPos = sf::Vector2f(position_.x + cornerSize, position_.y);
  auto rightPos = sf::Vector2f(position_.x + size_.x - cornerSize,
                               position_.y + cornerSize);
  auto bottomPos = sf::Vector2f(position_.x + cornerSize,
                                position_.y + size_.y - cornerSize);

  // DEBUG
  for (int i = 0; i < tileNum_; i++) {
    // Left
    auto leftTile = std::make_unique<StreetTilePanel>(
        leftPos + sf::Vector2f(0, i * size::regularTileWidth(tileNum_)),
        sf::Vector2f(cornerSize, size::regularTileWidth(tileNum_)), tileNum_,
        Orientation::Left, board::property::red, "YOGYAKARTA", "$ 100");
    addChild(std::move(leftTile));

    // Top
    auto topTile = std::make_unique<StreetTilePanel>(
        topPos + sf::Vector2f(i * size::regularTileWidth(tileNum_), 0),
        sf::Vector2f(size::regularTileWidth(tileNum_), cornerSize), tileNum_,
        Orientation::Top, board::property::yellow, "YOGYAKARTA", "$ 100");
    addChild(std::move(topTile));

    // Right
    auto rightTile = std::make_unique<StreetTilePanel>(
        rightPos + sf::Vector2f(0, i * size::regularTileWidth(tileNum_)),
        sf::Vector2f(cornerSize, size::regularTileWidth(tileNum_)), tileNum_,
        Orientation::Right, board::property::green, "YOGYAKARTA", "$ 100");
    addChild(std::move(rightTile));

    // Bottom
    auto bottomTile = std::make_unique<StreetTilePanel>(
        bottomPos + sf::Vector2f(i * size::regularTileWidth(tileNum_), 0),
        sf::Vector2f(size::regularTileWidth(tileNum_), cornerSize), tileNum_,
        Orientation::Bottom, board::property::darkBlue,
        "SANGATPANJANGTEKSNYAINI", "$ 100");
    addChild(std::move(bottomTile));
  }
}

void BoardPanel::handleEvent(sf::Event& event, sf::RenderWindow& window) {
  Panel::handleEvent(event, window);
}

void BoardPanel::render(sf::RenderWindow& window) { Panel::render(window); }

void BoardPanel::update(sf::RenderWindow& window) { Panel::update(window); }

}  // namespace ui
