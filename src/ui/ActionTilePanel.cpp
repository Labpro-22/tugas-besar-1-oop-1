#include "ui/ActionTilePanel.hpp"

#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <memory>

#include "ui/Constants.hpp"
#include "ui/Style.hpp"
#include "ui/TileGeometry.hpp"
#include "ui/TilePanel.hpp"
#include "ui/Widgets.hpp"

namespace ui {

sf::Vector2f ActionTilePanel::computePanelSize(int numTiles,
                                               Orientation orientation) {
  const TileGeometry geo(numTiles);
  const bool sideways =
      (orientation == Orientation::Left || orientation == Orientation::Right);
  return sideways ? sf::Vector2f(geo.sideHeight(), geo.sideWidth())
                  : sf::Vector2f(geo.sideWidth(), geo.sideHeight());
}

ActionTilePanel::ActionTilePanel(sf::Vector2f position, int numTiles,
                                 Orientation orientation,
                                 const std::string& iconPath,
                                 const std::string& topText,
                                 const std::string& bottomText)
    : TilePanel(position, computePanelSize(numTiles, orientation), numTiles,
                orientation),
      topText_(topText),
      bottomText_(bottomText),
      iconPath_(iconPath) {
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
}

LabelStyle topTextStyle() {
  return LabelStyle(typography::tileTopText, palette::black,
                    typography::regular, HorizontalAlign::Center,
                    VerticalAlign::Middle,
                    {layout::actionTilePanel::textPaddingX,
                     layout::actionTilePanel::textPaddingY},
                    true, typography::tileMinFont);
}

LabelStyle bottomTextStyle() {
  return LabelStyle(typography::tileBottomText, palette::black,
                    typography::regular, HorizontalAlign::Center,
                    VerticalAlign::Middle,
                    {layout::actionTilePanel::textPaddingX,
                     layout::actionTilePanel::textPaddingY},
                    true, typography::tileMinFont);
}

void ActionTilePanel::setupBottom() {
  const float textHeight = size_.y * layout::actionTilePanel::textHeightRatio;
  const auto bottomTextPos = sf::Vector2f(
      position_.x, position_.y + size_.y -
                       (size_.y * layout::actionTilePanel::textHeightRatio));

  const auto logoSize =
      sf::Vector2f(size_.x * layout::actionTilePanel::logoWidthRatio,
                   size_.y * layout::actionTilePanel::logoHeightRatio);
  const auto logoPos = sf::Vector2f(
      position_.x + (size_.x * layout::actionTilePanel::logoWidthOffset),
      position_.y +
          (size_.y *
           ((1.0f - layout::actionTilePanel::logoHeightRatio) / 2.0f)));

  topLabel_ = std::make_unique<Label>(
      topText_, font_, position_,
      sf::Vector2f(size_.x, std::max(0.0f, textHeight)), topTextStyle());

  iconPanel_ = std::make_unique<ImagePanel>(logoPos, logoSize, iconPath_);

  bottomLabel_ = std::make_unique<Label>(
      bottomText_, font_, bottomTextPos,
      sf::Vector2f(size_.x, std::max(0.0f, textHeight)), bottomTextStyle());

  topLabel_->setRotation(0.0f);
  bottomLabel_->setRotation(0.0f);
  iconPanel_->setRotation(0.0f);
}

void ActionTilePanel::setupLeft() {
  const float textWidth = size_.x * layout::actionTilePanel::textHeightRatio;
  const float topTextX = position_.x + size_.x - textWidth;
  const float logoWidth = size_.x * layout::actionTilePanel::logoHeightRatio;

  const auto logoSize = sf::Vector2f(
      logoWidth, size_.y * layout::actionTilePanel::logoWidthRatio);
  const auto logoPos = sf::Vector2f(
      position_.x + ((size_.x - logoWidth) / 2.0f),
      position_.y + (size_.y * layout::actionTilePanel::logoWidthOffset));

  topLabel_ = std::make_unique<Label>(
      topText_, font_, sf::Vector2f(topTextX, position_.y),
      sf::Vector2f(std::max(0.0f, textWidth), size_.y), topTextStyle());

  iconPanel_ = std::make_unique<ImagePanel>(logoPos, logoSize, iconPath_);

  bottomLabel_ = std::make_unique<Label>(
      bottomText_, font_, sf::Vector2f(position_.x, position_.y),
      sf::Vector2f(std::max(0.0f, textWidth), size_.y), bottomTextStyle());

  topLabel_->setRotation(90.0f);
  bottomLabel_->setRotation(90.0f);
  iconPanel_->setRotation(90.0f);
}

void ActionTilePanel::setupTop() {
  const float textHeight = size_.y * layout::actionTilePanel::textHeightRatio;
  const float topTextY = position_.y + size_.y - textHeight;

  const auto logoSize =
      sf::Vector2f(size_.x * layout::actionTilePanel::logoWidthRatio,
                   size_.y * layout::actionTilePanel::logoHeightRatio);
  const auto logoPos = sf::Vector2f(
      position_.x + (size_.x * layout::actionTilePanel::logoWidthOffset),
      position_.y +
          (size_.y *
           ((1.0f - layout::actionTilePanel::logoHeightRatio) / 2.0f)));

  topLabel_ = std::make_unique<Label>(
      topText_, font_, sf::Vector2f(position_.x, topTextY),
      sf::Vector2f(size_.x, std::max(0.0f, textHeight)), topTextStyle());

  iconPanel_ = std::make_unique<ImagePanel>(logoPos, logoSize, iconPath_);

  bottomLabel_ = std::make_unique<Label>(
      bottomText_, font_, sf::Vector2f(position_.x, position_.y),
      sf::Vector2f(size_.x, std::max(0.0f, textHeight)), bottomTextStyle());

  topLabel_->setRotation(180.0f);
  bottomLabel_->setRotation(180.0f);
  iconPanel_->setRotation(180.0f);
}

void ActionTilePanel::setupRight() {
  const float textWidth = size_.x * layout::actionTilePanel::textHeightRatio;
  const float topTextX = position_.x;
  const float logoWidth = size_.x * layout::actionTilePanel::logoHeightRatio;
  const float bottomTextX = position_.x + size_.x - textWidth;

  const auto logoSize = sf::Vector2f(
      logoWidth, size_.y * layout::actionTilePanel::logoWidthRatio);
  const auto logoPos = sf::Vector2f(
      position_.x + ((size_.x - logoWidth) / 2.0f),
      position_.y + (size_.y * layout::actionTilePanel::logoWidthOffset));

  topLabel_ = std::make_unique<Label>(
      topText_, font_, sf::Vector2f(topTextX, position_.y),
      sf::Vector2f(std::max(0.0f, textWidth), size_.y), topTextStyle());

  iconPanel_ = std::make_unique<ImagePanel>(logoPos, logoSize, iconPath_);

  bottomLabel_ = std::make_unique<Label>(
      bottomText_, font_, sf::Vector2f(bottomTextX, position_.y),
      sf::Vector2f(std::max(0.0f, (position_.x + size_.x) - bottomTextX),
                   size_.y),
      bottomTextStyle());

  topLabel_->setRotation(270.0f);
  bottomLabel_->setRotation(270.0f);
  iconPanel_->setRotation(270.0f);
}

void ActionTilePanel::handleEvent(sf::Event& event, sf::RenderWindow& window) {
  if (iconPanel_ && iconPanel_->isActive()) {
    iconPanel_->handleEvent(event, window);
  }
  if (topLabel_ && topLabel_->isActive()) {
    topLabel_->handleEvent(event, window);
  }
  if (bottomLabel_ && bottomLabel_->isActive()) {
    bottomLabel_->handleEvent(event, window);
  }

  Panel::handleEvent(event, window);
}

void ActionTilePanel::render(sf::RenderWindow& window) {
  if (!isVisible()) return;
  Panel::render(window);
  if (iconPanel_) {
    iconPanel_->render(window);
  }
  topLabel_->render(window);
  bottomLabel_->render(window);
}

void ActionTilePanel::update(sf::RenderWindow& window) {
  Panel::update(window);
  if (iconPanel_) {
    iconPanel_->update(window);
  }
  topLabel_->update(window);
  bottomLabel_->update(window);
}

}  // namespace ui