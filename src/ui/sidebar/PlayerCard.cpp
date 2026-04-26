#include "ui/sidebar/PlayerCard.hpp"

#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "core/Tiles.hpp"
#include "logic/Board.hpp"
#include "ui/AssetsManager.hpp"
#include "ui/component/Color.hpp"
#include "ui/component/Constants.hpp"

namespace {

bool isWhiteLike(const sf::Color& color) {
  return color == ui::board::property::white ||
         color == ui::board::property::defaultColor;
}

std::string avatarPathFor(core::Avatar avatar) {
  switch (avatar) {
    case core::Avatar::COPILOT:
      return "assets/players/copilot.png";
    case core::Avatar::CLAUDE:
      return "assets/players/claude.png";
    case core::Avatar::GEMINI:
      return "assets/players/gemini.png";
    case core::Avatar::CHATGPT:
      return "assets/players/openai.png";
    default:
      return "assets/players/copilot.png";
  }
}

std::string shortCode(const std::string& value) {
  std::string code;
  code.reserve(3);
  bool pickNext = true;

  for (char ch : value) {
    if (std::isspace(static_cast<unsigned char>(ch))) {
      pickNext = true;
      continue;
    }
    if (pickNext && std::isalpha(static_cast<unsigned char>(ch))) {
      code.push_back(static_cast<char>(
          std::toupper(static_cast<unsigned char>(ch))));
      pickNext = false;
      if (code.size() == 3) return code;
    }
  }

  if (code.empty()) {
    for (char ch : value) {
      if (std::isalnum(static_cast<unsigned char>(ch))) {
        code.push_back(static_cast<char>(
            std::toupper(static_cast<unsigned char>(ch))));
        if (code.size() == 3) break;
      }
    }
  }

  return code.empty() ? std::string("N/A") : code;
}

sf::Color actionTileColor(const core::ActionTile& tile) {
  switch (tile.getActionTileType()) {
    case core::ActionTileType::SPECIAL: {
      const auto* special = dynamic_cast<const core::SpecialTile*>(&tile);
      if (special != nullptr &&
          special->getSpecialTileType() == core::SpecialTileType::JAIL) {
        return ui::board::property::black;
      }
      return ui::board::property::white;
    }
    case core::ActionTileType::CARD:
      return ui::board::property::purple;
    case core::ActionTileType::TAX:
      return ui::board::property::red;
    case core::ActionTileType::FESTIVAL:
      return ui::board::property::orange;
    default:
      return ui::board::property::defaultColor;
  }
}

}  // namespace

namespace ui {

PropertyWidget::PropertyWidget(sf::Font& font)
    : font_(font), boxColor_(palette::white) {
  text_.setFont(font_);
  text_.setCharacterSize(typography::playerCardBody);
  text_.setStyle(typography::regular);
  text_.setFillColor(palette::black);
}

void PropertyWidget::setProperty(const PropertyView& property) {
  isOverflow_ = false;
  overflowCount_ = 0;
  boxColor_ = property.groupColor;

  text_.setString(property.code);
  text_.setFillColor(boxColor_ == board::property::black ? palette::white
                                                          : palette::black);

  hasIcon_ = false;
  iconTexture_ = nullptr;

  AssetsManager& assets = AssetsManager::get();
  if (property.isRailroad) {
    iconTexture_ = &assets.getTexture("assets/icons/train.png");
    hasIcon_ = true;
  } else if (property.isUtility && !property.utilityIconPath.empty()) {
    iconTexture_ = &assets.getTexture(property.utilityIconPath);
    hasIcon_ = true;
  } else if (property.houseCount >= 1 && property.houseCount <= 4) {
    iconTexture_ = &assets.getTexture("assets/icons/" +
                                      std::to_string(property.houseCount) +
                                      "-house.png");
    hasIcon_ = true;
  } else if (property.houseCount >= 5) {
    iconTexture_ = &assets.getTexture("assets/icons/hotel.png");
    hasIcon_ = true;
  }

  if (hasIcon_) {
    icon_.setTexture(*iconTexture_, true);
  }
}

void PropertyWidget::setOverflow(int remainingCount) {
  isOverflow_ = true;
  overflowCount_ = remainingCount;
  hasIcon_ = false;
  iconTexture_ = nullptr;
  text_.setString("+" + std::to_string(std::max(remainingCount, 0)));
  text_.setFillColor(palette::black);
  boxColor_ = sf::Color::Transparent;
}

void PropertyWidget::applyLayout(sf::Vector2f position, sf::Vector2f size,
                                 float iconHeightRatio) {
  position_ = position;
  size_ = size;

  const float iconHeight = size_.y * iconHeightRatio;
  const float boxHeight = std::max(1.0f, size_.y - iconHeight);

  box_.setPosition(position_);
  box_.setSize({size_.x, boxHeight});
  box_.setFillColor(boxColor_);

  if (!isOverflow_ && isWhiteLike(boxColor_)) {
    box_.setOutlineColor(palette::black);
    box_.setOutlineThickness(1.0f);
  } else {
    box_.setOutlineColor(sf::Color::Transparent);
    box_.setOutlineThickness(0.0f);
  }

  sf::FloatRect textBounds = text_.getLocalBounds();
  text_.setOrigin(textBounds.left + textBounds.width / 2.0f,
                  textBounds.top + textBounds.height / 2.0f);
  text_.setPosition(position_.x + size_.x / 2.0f,
                    position_.y + boxHeight / 2.0f);

  if (hasIcon_ && iconTexture_ != nullptr) {
    const sf::Vector2u source = iconTexture_->getSize();
    if (source.x > 0 && source.y > 0) {
      const float targetHeight = iconHeight * 0.85f;
      const float scale = targetHeight / static_cast<float>(source.y);
      icon_.setScale(scale, scale);
      sf::FloatRect iconBounds = icon_.getGlobalBounds();
      icon_.setPosition(
          position_.x + (size_.x - iconBounds.width) / 2.0f,
          position_.y + boxHeight + (iconHeight - iconBounds.height) / 2.0f);
    }
  }
}

void PropertyWidget::render(sf::RenderWindow& window) {
  if (!isVisible_) {
    return;
  }

  if (!isOverflow_) {
    window.draw(box_);
  }

  window.draw(text_);

  if (hasIcon_ && iconTexture_ != nullptr) {
    window.draw(icon_);
  }
}

PlayerCard::PlayerCard(sf::Vector2f position, sf::Vector2f size)
    : Widget(position, size),
      font_(AssetsManager::get().getFont(font::primaryFamily)),
      starTexture_(AssetsManager::get().getTexture("assets/icons/star.png")),
      mapPinBlackTexture_(AssetsManager::get().getTexture(
          "assets/icons/location_on_black.png")),
      mapPinWhiteTexture_(AssetsManager::get().getTexture(
          "assets/icons/location_on_white.png")) {
  background_.setFillColor(palette::white);

  avatarBackground_.setFillColor(palette::lightGrey);
  namePlateBackground_.setFillColor(palette::lightGrey);

  nameText_.setFont(font_);
  nameText_.setCharacterSize(typography::playerCardTitle);
  nameText_.setStyle(typography::titleStyle);
  nameText_.setFillColor(palette::black);

  turnBoxBackground_.setFillColor(palette::lightGrey);

  turnText_.setFont(font_);
  turnText_.setCharacterSize(typography::title);
  turnText_.setStyle(typography::titleStyle);
  turnText_.setFillColor(palette::black);

  divider_.setFillColor(palette::black);

  balanceText_.setFont(font_);
  balanceText_.setCharacterSize(typography::playerCardBody);
  balanceText_.setStyle(typography::titleStyle);
  balanceText_.setFillColor(palette::black);

  locationBoxBackground_.setFillColor(board::property::defaultColor);

  locationText_.setFont(font_);
  locationText_.setCharacterSize(typography::playerCardBody);
  locationText_.setStyle(typography::titleStyle);
  locationText_.setFillColor(palette::black);

  starSprite_.setTexture(starTexture_, true);
  mapPinSprite_.setTexture(mapPinBlackTexture_, true);

  updateLayout();
}

void PlayerCard::setPlayer(core::Player& player, const logic::Board& board,
                           bool isCurrentTurn, int turnNumber) {
  player_ = &player;

  nameText_.setString(player.getName());
  balanceText_.setString(formatBalance(player.getBalance()));
  turnText_.setString(std::to_string(turnNumber));
  showStar_ = isCurrentTurn;

  sf::Color tileColor = board::property::defaultColor;
  std::string tileCode = "N/A";

  core::Tile* tile = board.getTile(player.getPosition());
  if (tile != nullptr) {
    if (tile->getType() == core::TileType::PROPERTY) {
      auto* propertyTile = dynamic_cast<core::PropertyTile*>(tile);
      if (propertyTile != nullptr) {
        const PropertyView view = buildPropertyView(propertyTile->getProperty());
        tileColor = view.groupColor;
        tileCode = view.code;
      }
    } else {
      auto* actionTile = dynamic_cast<core::ActionTile*>(tile);
      if (actionTile != nullptr) {
        tileColor = actionTileColor(*actionTile);
      }
      tileCode = shortCode(tile->getName());
    }
  }

  locationText_.setString(tileCode);
  locationBoxBackground_.setFillColor(tileColor);
  locationBoxBackground_.setOutlineThickness(0.0f);
  locationBoxBackground_.setOutlineColor(sf::Color::Transparent);
  locationText_.setFillColor(palette::black);

  if (tileColor == board::property::white) {
    locationBoxBackground_.setOutlineColor(palette::black);
    locationBoxBackground_.setOutlineThickness(1.5f);
    mapPinSprite_.setTexture(mapPinBlackTexture_, true);
  } else if (tileColor == board::property::black) {
    locationText_.setFillColor(palette::white);
    mapPinSprite_.setTexture(mapPinWhiteTexture_, true);
  } else {
    mapPinSprite_.setTexture(mapPinBlackTexture_, true);
  }

  avatarTexture_ =
      &AssetsManager::get().getTexture(avatarPathFor(player.getAvatar()));
  avatarSprite_.setTexture(*avatarTexture_, true);

  std::vector<PropertyView> views;
  for (core::Property* property : player.getOwnedProperties()) {
    if (property != nullptr) {
      views.push_back(buildPropertyView(*property));
    }
  }
  rebuildPropertyGrid(views);
  updateLayout();
}

void PlayerCard::updateLayout() {
  background_.setPosition(position_);
  background_.setSize(size_);

  const float padding = layout::playerCard::padding;
  const float innerX = position_.x + padding;
  const float innerY = position_.y + padding;
  const float innerWidth = size_.x - (2.0f * padding);
  const float innerHeight = size_.y - (2.0f * padding);

  const float headerHeight = innerHeight * layout::playerCard::headerRatio;
  const float bodyHeight = innerHeight * layout::playerCard::propertyRatio;

  const float avatarSize = layout::playerCard::avatarSize;
  avatarBackground_.setPosition(innerX, innerY);
  avatarBackground_.setSize({avatarSize, avatarSize});

  if (avatarTexture_ != nullptr) {
    const sf::Vector2u avatarSource = avatarTexture_->getSize();
    if (avatarSource.x > 0 && avatarSource.y > 0) {
      const float sx = avatarSize / static_cast<float>(avatarSource.x);
      const float sy = avatarSize / static_cast<float>(avatarSource.y);
      const float scale = std::max(sx, sy);
      avatarSprite_.setScale(scale, scale);
      sf::FloatRect avatarBounds = avatarSprite_.getGlobalBounds();
      avatarSprite_.setPosition(
          innerX + (avatarSize - avatarBounds.width) / 2.0f,
          innerY + (avatarSize - avatarBounds.height) / 2.0f);
    }
  }

  const float col2X = innerX + avatarSize + padding * 0.8f;
  const float col2W = innerWidth - avatarSize - padding * 0.8f;
  const float turnBoxW = col2W * layout::playerCard::turnBoxWidthRatio;
  const float turnBoxH = avatarSize * 0.52f;

  namePlateBackground_.setPosition(col2X, innerY);
  namePlateBackground_.setSize({col2W - turnBoxW - padding * 0.3f, turnBoxH});

  sf::FloatRect nameBounds = nameText_.getLocalBounds();
  nameText_.setOrigin(nameBounds.left + nameBounds.width / 2.0f,
                      nameBounds.top + nameBounds.height / 2.0f);
  nameText_.setPosition(namePlateBackground_.getPosition().x +
                            namePlateBackground_.getSize().x / 2.0f,
                        namePlateBackground_.getPosition().y +
                            namePlateBackground_.getSize().y / 2.0f);

  turnBoxBackground_.setPosition(col2X + col2W - turnBoxW, innerY);
  turnBoxBackground_.setSize({turnBoxW, turnBoxH});

  sf::FloatRect turnBounds = turnText_.getLocalBounds();
  turnText_.setOrigin(turnBounds.left + turnBounds.width / 2.0f,
                      turnBounds.top + turnBounds.height / 2.0f);
  const float turnCenterX =
      turnBoxBackground_.getPosition().x + turnBoxBackground_.getSize().x / 2.0f;
  const float turnCenterY =
      turnBoxBackground_.getPosition().y + turnBoxBackground_.getSize().y / 2.0f;
  turnText_.setPosition(turnCenterX + (showStar_ ? turnBoxW * 0.12f : 0.0f),
                        turnCenterY);

  const sf::Vector2u starSource = starTexture_.getSize();
  if (starSource.x > 0 && starSource.y > 0) {
    const float starTarget = turnBoxH * 0.50f;
    const float starScale = starTarget / static_cast<float>(starSource.y);
    starSprite_.setScale(starScale, starScale);
    sf::FloatRect starBounds = starSprite_.getGlobalBounds();
    starSprite_.setPosition(turnCenterX - starBounds.width * 1.35f,
                            turnCenterY - starBounds.height / 2.0f);
  }

  divider_.setPosition(col2X, innerY + turnBoxH + padding * 0.35f);
  divider_.setSize({col2W, layout::playerCard::dividerThickness});

  const float financeY =
      divider_.getPosition().y + divider_.getSize().y + padding * 0.30f;
  const float financeH =
      std::max(12.0f,
               headerHeight - (financeY - innerY) - layout::playerCard::propertyGap);

  sf::FloatRect balanceBounds = balanceText_.getLocalBounds();
  balanceText_.setOrigin(balanceBounds.left,
                         balanceBounds.top + balanceBounds.height / 2.0f);
  balanceText_.setPosition(col2X, financeY + financeH / 2.0f);

  const float locationBoxW = std::max(col2W * 0.42f, 80.0f);
  locationBoxBackground_.setPosition(col2X + col2W - locationBoxW, financeY);
  locationBoxBackground_.setSize({locationBoxW, financeH});

  const sf::Texture& activePinTexture =
      locationText_.getFillColor() == palette::white ? mapPinWhiteTexture_
                                                     : mapPinBlackTexture_;
  const sf::Vector2u pinSource = activePinTexture.getSize();
  if (pinSource.x > 0 && pinSource.y > 0) {
    const float pinTarget = financeH * 0.65f;
    const float pinScale = pinTarget / static_cast<float>(pinSource.y);
    mapPinSprite_.setScale(pinScale, pinScale);
    sf::FloatRect pinBounds = mapPinSprite_.getGlobalBounds();
    mapPinSprite_.setPosition(
        locationBoxBackground_.getPosition().x + financeH * 0.18f,
        locationBoxBackground_.getPosition().y +
            (financeH - pinBounds.height) / 2.0f);

    sf::FloatRect locationBounds = locationText_.getLocalBounds();
    locationText_.setOrigin(locationBounds.left,
                            locationBounds.top + locationBounds.height / 2.0f);
    locationText_.setPosition(
        mapPinSprite_.getPosition().x + pinBounds.width + financeH * 0.10f,
        locationBoxBackground_.getPosition().y + financeH / 2.0f);
  }

  const float propertiesY =
      innerY + headerHeight + layout::playerCard::propertyGap;
  const float propertiesW = innerWidth;
  const float propertiesH = bodyHeight - layout::playerCard::propertyGap;
  const float cellW =
      (propertiesW -
       (layout::playerCard::propertyColumns - 1) * layout::playerCard::propertyGap) /
      static_cast<float>(layout::playerCard::propertyColumns);
  const float cellH = (propertiesH - layout::playerCard::propertyGap) / 2.0f;

  for (size_t i = 0; i < propertyWidgets_.size(); ++i) {
    const int col = static_cast<int>(i) % layout::playerCard::propertyColumns;
    const int row = static_cast<int>(i) / layout::playerCard::propertyColumns;
    const float x = innerX + col * (cellW + layout::playerCard::propertyGap);
    const float y =
        propertiesY + row * (cellH + layout::playerCard::propertyGap);
    propertyWidgets_[i].applyLayout(
        {x, y}, {cellW, cellH},
        std::min(layout::playerCard::propertyIconHeightRatioMax,
                 layout::playerCard::propertyIconHeight / cellH));
  }
}

void PlayerCard::rebuildPropertyGrid(const std::vector<PropertyView>& properties) {
  propertyWidgets_.clear();

  const int maxSlots = layout::playerCard::maxPropertySlots;
  const int count = static_cast<int>(properties.size());
  if (count <= maxSlots) {
    propertyWidgets_.reserve(properties.size());
    for (const PropertyView& property : properties) {
      PropertyWidget widget(font_);
      widget.setProperty(property);
      propertyWidgets_.push_back(std::move(widget));
    }
    return;
  }

  propertyWidgets_.reserve(maxSlots);
  for (int i = 0; i < maxSlots - 1; ++i) {
    PropertyWidget widget(font_);
    widget.setProperty(properties[static_cast<size_t>(i)]);
    propertyWidgets_.push_back(std::move(widget));
  }

  PropertyWidget overflowWidget(font_);
  overflowWidget.setOverflow(count - (maxSlots - 1));
  propertyWidgets_.push_back(std::move(overflowWidget));
}

std::string PlayerCard::formatBalance(long long balance) {
  const bool negative = balance < 0;
  unsigned long long value =
      static_cast<unsigned long long>(negative ? -balance : balance);

  std::string digits = std::to_string(value);
  std::string grouped;
  grouped.reserve(digits.size() + digits.size() / 3);

  for (size_t i = 0; i < digits.size(); ++i) {
    if (i > 0 && (digits.size() - i) % 3 == 0) {
      grouped.push_back(',');
    }
    grouped.push_back(digits[i]);
  }

  std::ostringstream out;
  out << "$ ";
  if (negative) {
    out << '-';
  }
  out << grouped;
  return out.str();
}

void PlayerCard::render(sf::RenderWindow& window) {
  if (!isVisible_) {
    return;
  }

  window.draw(background_);
  window.draw(avatarBackground_);
  if (avatarTexture_ != nullptr) {
    window.draw(avatarSprite_);
  }
  window.draw(namePlateBackground_);
  window.draw(nameText_);
  window.draw(turnBoxBackground_);
  if (showStar_) {
    window.draw(starSprite_);
  }
  window.draw(turnText_);
  window.draw(divider_);
  window.draw(balanceText_);
  window.draw(locationBoxBackground_);
  window.draw(mapPinSprite_);
  window.draw(locationText_);
  for (PropertyWidget& widget : propertyWidgets_) {
    widget.render(window);
  }
}

}  // namespace ui
