#include "ui/board/TilePanel.hpp"

#include "ui/AssetsManager.hpp"
#include "ui/component/Constants.hpp"
#include "ui/component/Style.hpp"

namespace ui {

namespace {
ButtonStyle transparentTileButtonStyle() {
  const LabelStyle hiddenLabel(typography::tileMinFont, sf::Color::Transparent,
                               typography::regular, HorizontalAlign::Center,
                               VerticalAlign::Middle, {0.0f, 0.0f}, false,
                               typography::tileMinFont);
  return ButtonStyle(sf::Color::Transparent, palette::black20,
                     sf::Color::Transparent, sf::Color::Transparent,
                     sf::Color::Transparent, 0.0f, hiddenLabel);
}
}  // namespace

TilePanel::TilePanel(sf::Vector2f position, sf::Vector2f panelSize,
                     int numTiles, Orientation orientation)
    : Panel(position, panelSize,
            PanelStyle(board::base, palette::black, size::lineThickness)),
      orientation_(orientation),
      tileGeo_(numTiles),
      font_(AssetsManager::get().getFont(font::primaryFamily)),
      interactionButton_(std::make_unique<Button>(
          "", font_, position_, size_, transparentTileButtonStyle(), [this]() {
            if (onSelected_) {
              onSelected_(selectionInfo_);
            }
          })) {}

sf::Vector2f TilePanel::computeSidePanelSize(int numTiles,
                                             Orientation orientation) {
  const TileGeometry geo(numTiles);
  const bool sideways =
      (orientation == Orientation::Left || orientation == Orientation::Right);
  return sideways ? sf::Vector2f(geo.sideHeight(), geo.sideWidth())
                  : sf::Vector2f(geo.sideWidth(), geo.sideHeight());
}

void TilePanel::setOnSelected(std::function<void(const TileInfo&)> onSelected) {
  onSelected_ = std::move(onSelected);
  if (interactionButton_) {
    interactionButton_->setOnClick([this]() {
      if (onSelected_) {
        onSelected_(selectionInfo_);
      }
    });
  }
}

void TilePanel::handleEvent(sf::Event& event, sf::RenderWindow& window) {
  if (interactionButton_ && interactionButton_->isActive()) {
    interactionButton_->handleEvent(event, window);
  }
}

void TilePanel::render(sf::RenderWindow& window) {
  if (interactionButton_) {
    interactionButton_->render(window);
  }
}

void TilePanel::update(sf::RenderWindow& window) {
  if (!interactionButton_) {
    return;
  }
  interactionButton_->setPosition(position_);
  interactionButton_->setSize(size_);
  interactionButton_->update(window);
}

}  // namespace ui
