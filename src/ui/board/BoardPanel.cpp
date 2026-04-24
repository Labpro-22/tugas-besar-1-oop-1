#include "ui/board/BoardPanel.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <stdexcept>
#include <utility>

#include "ui/board/ActionTilePanel.hpp"
#include "ui/board/CornerTilePanel.hpp"
#include "ui/board/StreetTilePanel.hpp"
#include "ui/board/TileGeometry.hpp"
#include "ui/board/TilePanel.hpp"
#include "ui/component/Color.hpp"
#include "ui/component/Constants.hpp"
#include "ui/component/Widgets.hpp"

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

void BoardPanel::setOnTileSelected(
    std::function<void(const TileInfo&)> onTileSelected) {
  onTileSelected_ = std::move(onTileSelected);

  for (auto& entry : children_) {
    auto* tile = dynamic_cast<TilePanel*>(std::get<2>(entry).get());
    if (tile) {
      tile->setOnSelected(onTileSelected_);
    }
  }
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
    auto leftTile = std::make_unique<ActionTilePanel>(
        leftPos + sf::Vector2f(0, i * tileWidth), tileNum_, Orientation::Left,
        "assets/icons/train.png", "BANDUNG", "$ 100");
    leftTile->setOnSelected(onTileSelected_);
    addChild(std::move(leftTile));

    // Top
    auto topTile = std::make_unique<ActionTilePanel>(
        topPos + sf::Vector2f(i * tileWidth, 0), tileNum_, Orientation::Top,
        "assets/icons/chance-pink.png", "KESEMPATAN", "$ 100");
    topTile->setOnSelected(onTileSelected_);
    addChild(std::move(topTile));

    // Right
    auto rightTile = std::make_unique<StreetTilePanel>(
        rightPos + sf::Vector2f(0, i * tileWidth), tileNum_, Orientation::Right,
        board::property::red, "JAKARTA", "$ 100");
    rightTile->setOnSelected(onTileSelected_);
    addChild(std::move(rightTile));

    // Bottom
    auto bottomTile = std::make_unique<StreetTilePanel>(
        bottomPos + sf::Vector2f(i * tileWidth, 0), tileNum_,
        Orientation::Bottom, board::property::lavender, "SOLO", "$ 100");
    bottomTile->setOnSelected(onTileSelected_);
    addChild(std::move(bottomTile));
  }

  auto goTile = std::make_unique<CornerTilePanel>(
      position_ + size_ - sf::Vector2f(cornerSize, cornerSize), tileNum_,
      CornerType::GO, Orientation::Bottom, "assets/icons/go.png", "MULAI",
      "DAPAT UANG");
  goTile->setOnSelected(onTileSelected_);
  addChild(std::move(goTile));

  auto jailTile = std::make_unique<CornerTilePanel>(
      position_ + sf::Vector2f(0, size_.y - cornerSize), tileNum_,
      CornerType::JAIL, Orientation::Left, "assets/icons/jail.png", "HANYA",
      "BERKUNJUNG");
  jailTile->setOnSelected(onTileSelected_);
  addChild(std::move(jailTile));

  auto freeTile = std::make_unique<CornerTilePanel>(
      position_, tileNum_, CornerType::FREE, Orientation::Top,
      "assets/icons/free-park.png", "PARKIR", "GRATIS");
  freeTile->setOnSelected(onTileSelected_);
  addChild(std::move(freeTile));

  auto goToJailTile = std::make_unique<CornerTilePanel>(
      position_ + sf::Vector2f(size_.x - cornerSize, 0), tileNum_,
      CornerType::GO_TO_JAIL, Orientation::Right, "assets/icons/go-to-jail.png",
      "PERGI", "KE PENJARA");
  goToJailTile->setOnSelected(onTileSelected_);
  addChild(std::move(goToJailTile));
}

void BoardPanel::handleEvent(sf::Event& event, sf::RenderWindow& window) {
  Panel::handleEvent(event, window);
}

void BoardPanel::render(sf::RenderWindow& window) { Panel::render(window); }

void BoardPanel::update(sf::RenderWindow& window) { Panel::update(window); }

}  // namespace ui
