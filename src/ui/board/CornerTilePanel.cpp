#include "ui/board/CornerTilePanel.hpp"

#include <SFML/System/Vector2.hpp>

#include "ui/board/TileGeometry.hpp"
#include "ui/component/Constants.hpp"
#include "ui/component/Style.hpp"

namespace ui {

CornerTilePanel::CornerTilePanel(sf::Vector2f position, int numTiles,
                                 CornerType cornerType, Orientation orientation,
                                 const std::string& iconPath,
                                 const std::string& topText,
                                 const std::string& bottomText)
    : TilePanel(position, TileGeometry(numTiles).cornerSize(), numTiles,
                orientation),
      cornerType_(cornerType),
      topText_(topText),
      bottomText_(bottomText),
      iconPath_(iconPath) {
  if (cornerType_ == CornerType::JAIL) {
    setupJail();
  } else {
    setupNonJail();
  }

  switch (cornerType_) {
    case CornerType::GO:
      selectionInfo_ = TileInfo{"MULAI", iconPath_, false, board::base, false};
      break;
    case CornerType::JAIL:
      selectionInfo_ =
          TileInfo{"PENJARA", iconPath_, false, board::base, false};
      break;
    case CornerType::FREE:
      selectionInfo_ =
          TileInfo{"BEBAS PARKIR", iconPath_, false, board::base, false};
      break;
    case CornerType::GO_TO_JAIL:
      selectionInfo_ =
          TileInfo{"PERGI KE PENJARA", iconPath_, false, board::base, false};
      break;
  }
}

sf::Vector2f CornerTilePanel::computeTopTextPos() {
  if (cornerType_ == CornerType::JAIL) {
    const float bandW = size_.x * layout::cornerTilePanel::jailTextBandRatio;
    const float bandH = size_.y * layout::cornerTilePanel::jailTextBandRatio;
    switch (orientation_) {
      case Orientation::Bottom:
        return {position_.x, position_.y + size_.y - bandH};
      case Orientation::Left:
        return {position_.x, position_.y};
      case Orientation::Top:
        return {position_.x, position_.y};
      case Orientation::Right:
      default:
        return {position_.x + size_.x - bandW, position_.y};
    }
  }

  const float insetX = size_.x * layout::cornerTilePanel::diagonalInsetRatio;
  const float insetY = size_.y * layout::cornerTilePanel::diagonalInsetRatio;
  const auto textSize = computeTopTextSize();

  switch (orientation_) {
    case Orientation::Bottom:
      return {position_.x + insetX, position_.y + insetY};
    case Orientation::Left:
      return {position_.x + size_.x - insetX - textSize.x,
              position_.y + insetY};
    case Orientation::Top:
      return {position_.x + size_.x - insetX - textSize.x,
              position_.y + size_.y - insetY - textSize.y};
    case Orientation::Right:
    default:
      return {position_.x + insetX,
              position_.y + size_.y - insetY - textSize.y};
  }
}

sf::Vector2f CornerTilePanel::computeTopTextSize() const {
  if (cornerType_ == CornerType::JAIL) {
    const float bandW = size_.x * layout::cornerTilePanel::jailTextBandRatio;
    const float bandH = size_.y * layout::cornerTilePanel::jailTextBandRatio;
    switch (orientation_) {
      case Orientation::Bottom:
      case Orientation::Top:
        return {size_.x, bandH};
      case Orientation::Left:
      case Orientation::Right:
      default:
        return {bandW, size_.y};
    }
  }

  return {size_.x * layout::cornerTilePanel::nonJailTextWidthRatio,
          size_.y * layout::cornerTilePanel::nonJailTextHeightRatio};
}

sf::Vector2f CornerTilePanel::computeIconPos() {
  const auto iconSize = computeIconSize();
  if (cornerType_ != CornerType::JAIL) {
    return {position_.x + ((size_.x - iconSize.x) / 2.0f),
            position_.y + ((size_.y - iconSize.y) / 2.0f)};
  }

  const float insetX = size_.x * layout::cornerTilePanel::jailLogoInsetRatio;
  const float insetY = size_.y * layout::cornerTilePanel::jailLogoInsetRatio;
  switch (orientation_) {
    case Orientation::Bottom:
      return {position_.x + insetX, position_.y + insetY};
    case Orientation::Left:
      return {position_.x + size_.x - insetX - iconSize.x,
              position_.y + insetY};
    case Orientation::Top:
      return {position_.x + insetX,
              position_.y + size_.y - insetY - iconSize.y};
    case Orientation::Right:
    default:
      return {position_.x + size_.x - insetX - iconSize.x,
              position_.y + size_.y - insetY - iconSize.y};
  }
}

sf::Vector2f CornerTilePanel::computeIconSize() const {
  const float ratio = (cornerType_ == CornerType::JAIL)
                          ? layout::cornerTilePanel::jailLogoSizeRatio
                          : layout::cornerTilePanel::logoSizeRatio;
  return {size_.x * ratio, size_.y * ratio};
}

sf::Vector2f CornerTilePanel::computeBottomTextPos() {
  if (cornerType_ == CornerType::JAIL) {
    const float bandW = size_.x * layout::cornerTilePanel::jailTextBandRatio;
    const float bandH = size_.y * layout::cornerTilePanel::jailTextBandRatio;
    switch (orientation_) {
      case Orientation::Bottom:
        return {position_.x + size_.x - bandW, position_.y};
      case Orientation::Left:
        return {position_.x, position_.y + size_.y - bandH};
      case Orientation::Top:
        return {position_.x, position_.y};
      case Orientation::Right:
      default:
        return {position_.x, position_.y};
    }
  }

  const float insetX = size_.x * layout::cornerTilePanel::diagonalInsetRatio;
  const float insetY = size_.y * layout::cornerTilePanel::diagonalInsetRatio;
  const auto textSize = computeBottomTextSize();
  switch (orientation_) {
    case Orientation::Bottom:
      return {position_.x + size_.x - insetX - textSize.x,
              position_.y + size_.y - insetY - textSize.y};
    case Orientation::Left:
      return {position_.x + insetX,
              position_.y + size_.y - insetY - textSize.y};
    case Orientation::Top:
      return {position_.x + insetX, position_.y + insetY};
    case Orientation::Right:
    default:
      return {position_.x + size_.x - insetX - textSize.x,
              position_.y + insetY};
  }
}

sf::Vector2f CornerTilePanel::computeBottomTextSize() const {
  if (cornerType_ == CornerType::JAIL) {
    const float bandW = size_.x * layout::cornerTilePanel::jailTextBandRatio;
    const float bandH = size_.y * layout::cornerTilePanel::jailTextBandRatio;
    switch (orientation_) {
      case Orientation::Bottom:
      case Orientation::Top:
        return {bandW, size_.y};
      case Orientation::Left:
      case Orientation::Right:
      default:
        return {size_.x, bandH};
    }
  }

  return {size_.x * layout::cornerTilePanel::nonJailTextWidthRatio,
          size_.y * layout::cornerTilePanel::nonJailTextHeightRatio};
}

namespace {
LabelStyle textStyle() {
  return LabelStyle(typography::tileTopText, palette::black,
                    typography::regular, HorizontalAlign::Center,
                    VerticalAlign::Middle,
                    {layout::cornerTilePanel::textPaddingX,
                     layout::cornerTilePanel::textPaddingY},
                    true, typography::tileMinFont);
}
}  // namespace
void CornerTilePanel::setupNonJail() {
  const auto topPos = computeTopTextPos();
  const auto topSize = computeTopTextSize();
  const auto iconPos = computeIconPos();
  const auto iconSize = computeIconSize();
  const auto bottomPos = computeBottomTextPos();
  const auto bottomSize = computeBottomTextSize();

  topLabel_ =
      std::make_unique<Label>(topText_, font_, topPos, topSize, textStyle());
  iconPanel_ = std::make_unique<ImagePanel>(iconPos, iconSize, iconPath_);
  bottomLabel_ = std::make_unique<Label>(bottomText_, font_, bottomPos,
                                         bottomSize, textStyle());

  float rotation = 315.0f;
  switch (orientation_) {
    case Orientation::Bottom:
      rotation = 315.0f;
      break;
    case Orientation::Left:
      rotation = 45.0f;
      break;
    case Orientation::Top:
      rotation = 135.0f;
      break;
    case Orientation::Right:
      rotation = 225.0f;
      break;
  }

  topLabel_->setRotation(rotation);
  iconPanel_->setRotation(rotation);
  bottomLabel_->setRotation(rotation);
}

void CornerTilePanel::setupJail() {
  const auto topPos = computeTopTextPos();
  const auto topSize = computeTopTextSize();
  const auto iconPos = computeIconPos();
  const auto iconSize = computeIconSize();
  const auto bottomPos = computeBottomTextPos();
  const auto bottomSize = computeBottomTextSize();

  topLabel_ =
      std::make_unique<Label>(topText_, font_, topPos, topSize, textStyle());
  iconPanel_ = std::make_unique<ImagePanel>(iconPos, iconSize, iconPath_);
  bottomLabel_ = std::make_unique<Label>(bottomText_, font_, bottomPos,
                                         bottomSize, textStyle());

  float topRotation = 0.0f;
  float bottomRotation = 0.0f;
  switch (orientation_) {
    case Orientation::Bottom:
      topRotation = 0.0f;
      bottomRotation = 270.0f;
      break;
    case Orientation::Left:
      topRotation = 90.0f;
      bottomRotation = 0.0f;
      break;
    case Orientation::Top:
      topRotation = 180.0f;
      bottomRotation = 90.0f;
      break;
    case Orientation::Right:
      topRotation = 270.0f;
      bottomRotation = 180.0f;
      break;
  }

  topLabel_->setRotation(topRotation);
  iconPanel_->setRotation(0.0f);
  bottomLabel_->setRotation(bottomRotation);
}

void CornerTilePanel::handleEvent(sf::Event& event, sf::RenderWindow& window) {
  TilePanel::handleEvent(event, window);

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

void CornerTilePanel::render(sf::RenderWindow& window) {
  if (!isVisible()) return;
  Panel::render(window);
  if (iconPanel_) {
    iconPanel_->render(window);
  }
  if (topLabel_) {
    topLabel_->render(window);
  }
  if (bottomLabel_) {
    bottomLabel_->render(window);
  }
  TilePanel::render(window);
}

void CornerTilePanel::update(sf::RenderWindow& window) {
  Panel::update(window);
  if (iconPanel_) {
    iconPanel_->update(window);
  }
  if (topLabel_) {
    topLabel_->update(window);
  }
  if (bottomLabel_) {
    bottomLabel_->update(window);
  }
  TilePanel::update(window);
}

}  // namespace ui