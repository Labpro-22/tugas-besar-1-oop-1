#include "ui/board/StreetTilePanel.hpp"

#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <memory>

#include "ui/board/TileGeometry.hpp"
#include "ui/board/TilePanel.hpp"
#include "ui/component/Color.hpp"
#include "ui/component/Constants.hpp"
#include "ui/component/Style.hpp"
#include "ui/component/Widgets.hpp"

namespace ui {

StreetTilePanel::StreetTilePanel(sf::Vector2f position, int numTiles,
                                 Orientation orientation, sf::Color accentColor,
                                 const std::string& topText,
                                 const std::string& bottomText)
    : TilePanel(position, computeSidePanelSize(numTiles, orientation), numTiles,
                orientation),
      accentColor_(accentColor),
      topText_(topText),
      bottomText_(bottomText) {
  switch (orientation_) {
    case Orientation::Bottom:
      setupBottom();
      break;
    case Orientation::Left:
      setupLeft();
      break;
    case Orientation::Top:
      setupTop();
      break;
    case Orientation::Right:
      setupRight();
      break;
  }

  selectionInfo_ = TileInfo{topText_, bottomText_, true, accentColor_, true};
}

float StreetTilePanel::tileAccentHeight() const {
  return tileGeo_.sideHeight() * 0.2f;
}

namespace {
LabelStyle topTextStyle() {
  return LabelStyle(typography::tileTopText, palette::black,
                    typography::regular, HorizontalAlign::Center,
                    VerticalAlign::Middle,
                    {layout::streetTilePanel::textPaddingX,
                     layout::streetTilePanel::textPaddingY},
                    true, typography::tileMinFont);
}

LabelStyle bottomTextStyle() {
  return LabelStyle(typography::tileBottomText, palette::black,
                    typography::regular, HorizontalAlign::Center,
                    VerticalAlign::Middle,
                    {layout::streetTilePanel::textPaddingX,
                     layout::streetTilePanel::textPaddingY},
                    true, typography::tileMinFont);
}
}  // namespace

void StreetTilePanel::setupBottom() {
  const float accentHeight = tileAccentHeight();
  const float topTextY =
      position_.y + (size_.y * layout::streetTilePanel::topTextOffsetRatio);
  const float bottomTextY =
      position_.y + (size_.y * layout::streetTilePanel::bottomTextOffsetRatio);

  accentPanel_ = std::make_unique<Panel>(
      position_, sf::Vector2f(size_.x, accentHeight),
      PanelStyle(accentColor_, palette::black,
                 layout::streetTilePanel::borderThickness));

  topLabel_ = std::make_unique<Label>(
      topText_, font_, sf::Vector2f(position_.x, topTextY),
      sf::Vector2f(size_.x, std::max(0.0f, bottomTextY - topTextY)),
      topTextStyle());

  bottomLabel_ = std::make_unique<Label>(
      bottomText_, font_, sf::Vector2f(position_.x, bottomTextY),
      sf::Vector2f(size_.x,
                   std::max(0.0f, (position_.y + size_.y) - bottomTextY)),
      bottomTextStyle());

  topLabel_->setRotation(0.0f);
  bottomLabel_->setRotation(0.0f);
}

void StreetTilePanel::setupLeft() {
  const float accentWidth = tileAccentHeight();
  const float topTextX =
      position_.x +
      (size_.x * (1.0f - layout::streetTilePanel::bottomTextOffsetRatio));
  const float bottomTextX =
      position_.x +
      (size_.x * (1.0f - layout::streetTilePanel::topTextOffsetRatio));

  accentPanel_ = std::make_unique<Panel>(
      sf::Vector2f(position_.x + size_.x - accentWidth, position_.y),
      sf::Vector2f(accentWidth, size_.y),
      PanelStyle(accentColor_, palette::black,
                 layout::streetTilePanel::borderThickness));

  topLabel_ = std::make_unique<Label>(
      topText_, font_, sf::Vector2f(topTextX, position_.y),
      sf::Vector2f(std::max(0.0f, bottomTextX - topTextX), size_.y),
      topTextStyle());

  bottomLabel_ = std::make_unique<Label>(
      bottomText_, font_, sf::Vector2f(position_.x, position_.y),
      sf::Vector2f(std::max(0.0f, topTextX - position_.x), size_.y),
      bottomTextStyle());

  topLabel_->setRotation(90.0f);
  bottomLabel_->setRotation(90.0f);
}

void StreetTilePanel::setupTop() {
  const float accentHeight = tileAccentHeight();
  const float topTextY =
      position_.y +
      (size_.y * (1.0f - layout::streetTilePanel::bottomTextOffsetRatio));
  const float bottomTextY =
      position_.y +
      (size_.y * (1.0f - layout::streetTilePanel::topTextOffsetRatio));

  accentPanel_ = std::make_unique<Panel>(
      sf::Vector2f(position_.x, position_.y + size_.y - accentHeight),
      sf::Vector2f(size_.x, accentHeight),
      PanelStyle(accentColor_, palette::black,
                 layout::streetTilePanel::borderThickness));

  topLabel_ = std::make_unique<Label>(
      topText_, font_, sf::Vector2f(position_.x, topTextY),
      sf::Vector2f(size_.x, std::max(0.0f, bottomTextY - topTextY)),
      topTextStyle());

  bottomLabel_ = std::make_unique<Label>(
      bottomText_, font_, sf::Vector2f(position_.x, position_.y),
      sf::Vector2f(size_.x, std::max(0.0f, topTextY - position_.y)),
      bottomTextStyle());

  topLabel_->setRotation(180.0f);
  bottomLabel_->setRotation(180.0f);
}

void StreetTilePanel::setupRight() {
  const float accentWidth = tileAccentHeight();
  const float topTextX =
      position_.x + (size_.x * layout::streetTilePanel::topTextOffsetRatio);
  const float bottomTextX =
      position_.x + (size_.x * layout::streetTilePanel::bottomTextOffsetRatio);

  accentPanel_ = std::make_unique<Panel>(
      sf::Vector2f(position_.x, position_.y),
      sf::Vector2f(accentWidth, size_.y),
      PanelStyle(accentColor_, palette::black,
                 layout::streetTilePanel::borderThickness));

  topLabel_ = std::make_unique<Label>(
      topText_, font_, sf::Vector2f(topTextX, position_.y),
      sf::Vector2f(std::max(0.0f, bottomTextX - topTextX), size_.y),
      topTextStyle());

  bottomLabel_ = std::make_unique<Label>(
      bottomText_, font_, sf::Vector2f(bottomTextX, position_.y),
      sf::Vector2f(std::max(0.0f, (position_.x + size_.x) - bottomTextX),
                   size_.y),
      bottomTextStyle());

  topLabel_->setRotation(270.0f);
  bottomLabel_->setRotation(270.0f);
}

void StreetTilePanel::handleEvent(sf::Event& event, sf::RenderWindow& window) {
  TilePanel::handleEvent(event, window);

  if (accentPanel_ && accentPanel_->isActive()) {
    accentPanel_->handleEvent(event, window);
  }
  if (topLabel_ && topLabel_->isActive()) {
    topLabel_->handleEvent(event, window);
  }
  if (bottomLabel_ && bottomLabel_->isActive()) {
    bottomLabel_->handleEvent(event, window);
  }

  Panel::handleEvent(event, window);
}

void StreetTilePanel::render(sf::RenderWindow& window) {
  if (!isVisible()) return;
  Panel::render(window);
  accentPanel_->render(window);
  topLabel_->render(window);
  bottomLabel_->render(window);
  TilePanel::render(window);
}

void StreetTilePanel::update(sf::RenderWindow& window) {
  Panel::update(window);
  accentPanel_->update(window);
  topLabel_->update(window);
  bottomLabel_->update(window);
  TilePanel::update(window);
}

}  // namespace ui