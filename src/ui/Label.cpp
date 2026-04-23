#include <algorithm>
#include <cmath>

#include "ui/Widgets.hpp"

namespace ui {

Label::Label(const std::string& text, const sf::Font& font,
             const LabelStyle& style)
    : Widget(), style_(style), text_(text, font, style.characterSize()) {
  applyStyle();
}

Label::Label(const std::string& text, const sf::Font& font,
             sf::Vector2f position, sf::Vector2f size, const LabelStyle& style)
    : Widget(position, size),
      style_(style),
      text_(text, font, style.characterSize()) {
  applyStyle();
}

Label::~Label() = default;

void Label::setText(const std::string& text) { text_.setString(text); }

void Label::setRotation(float degrees) { rotationDegrees_ = degrees; }

void Label::setStyle(const LabelStyle& style) {
  style_ = style;
  applyStyle();
}

void Label::applyStyle() {
  text_.setFillColor(style_.color());
  text_.setStyle(style_.textStyle());
  text_.setCharacterSize(style_.characterSize());
}

void Label::render(sf::RenderWindow& window) {
  if (!isVisible()) return;

  applyStyle();
  const float normalizedRotation =
      std::fmod(std::fmod(rotationDegrees_, 360.0f) + 360.0f, 360.0f);
  const bool quarterTurn = std::fabs(normalizedRotation - 90.0f) < 0.1f ||
                           std::fabs(normalizedRotation - 270.0f) < 0.1f;

  auto textBounds = text_.getLocalBounds();

  if (style_.autoScaleToFit() && size().x > 0.0f) {
    const float availablePrimary = quarterTurn
                                       ? size().y - (2.0f * style_.padding().y)
                                       : size().x - (2.0f * style_.padding().x);
    const float maxWidth = std::max(0.0f, availablePrimary);
    if (textBounds.width > maxWidth && textBounds.width > 0.0f) {
      const float scale = maxWidth / textBounds.width;
      const float candidate =
          std::floor(static_cast<float>(style_.characterSize()) * scale);
      const auto fittedSize = static_cast<unsigned int>(
          std::max(static_cast<float>(style_.minCharacterSize()), candidate));
      text_.setCharacterSize(fittedSize);
      textBounds = text_.getLocalBounds();
    }
  }

  const float renderedWidth =
      quarterTurn ? textBounds.height : textBounds.width;
  const float renderedHeight =
      quarterTurn ? textBounds.width : textBounds.height;

  float drawX = position().x;
  float drawY = position().y;

  if (size().x > 0.0f) {
    const float contentX = position().x + style_.padding().x;
    const float contentWidth =
        std::max(0.0f, size().x - (2.0f * style_.padding().x));
    switch (style_.horizontalAlign()) {
      case HorizontalAlign::Left:
        drawX = contentX;
        break;
      case HorizontalAlign::Center:
        drawX = contentX + ((contentWidth - renderedWidth) * 0.5f);
        break;
      case HorizontalAlign::Right:
        drawX = contentX + contentWidth - renderedWidth;
        break;
    }
  }

  if (size().y > 0.0f) {
    const float contentY = position().y + style_.padding().y;
    const float contentHeight =
        std::max(0.0f, size().y - (2.0f * style_.padding().y));
    switch (style_.verticalAlign()) {
      case VerticalAlign::Top:
        drawY = contentY;
        break;
      case VerticalAlign::Middle:
        drawY = contentY + ((contentHeight - renderedHeight) * 0.5f);
        break;
      case VerticalAlign::Bottom:
        drawY = contentY + contentHeight - renderedHeight;
        break;
    }
  }

  text_.setOrigin(textBounds.left + (textBounds.width * 0.5f),
                  textBounds.top + (textBounds.height * 0.5f));
  text_.setRotation(rotationDegrees_);
  text_.setPosition(drawX + (renderedWidth * 0.5f),
                    drawY + (renderedHeight * 0.5f));
  window.draw(text_);
}

}  // namespace ui
