#include "ui/Style.hpp"

namespace ui {

LabelStyle::LabelStyle()
    : LabelStyle(30, sf::Color::White, sf::Text::Regular, HorizontalAlign::Left,
                 VerticalAlign::Top, sf::Vector2f(0.0f, 0.0f), false, 10) {}

LabelStyle::LabelStyle(unsigned int characterSize, sf::Color color,
                       sf::Uint32 textStyle, HorizontalAlign horizontalAlign,
                       VerticalAlign verticalAlign, sf::Vector2f padding,
                       bool autoScaleToFit, unsigned int minCharacterSize)
    : characterSize_(characterSize),
      color_(color),
      textStyle_(textStyle),
      horizontalAlign_(horizontalAlign),
      verticalAlign_(verticalAlign),
      padding_(padding),
      autoScaleToFit_(autoScaleToFit),
      minCharacterSize_(minCharacterSize) {}

LabelStyle& LabelStyle::setAlignment(HorizontalAlign horizontalAlign,
                                     VerticalAlign verticalAlign) {
  horizontalAlign_ = horizontalAlign;
  verticalAlign_ = verticalAlign;
  return *this;
}

LabelStyle& LabelStyle::setPadding(sf::Vector2f padding) {
  padding_ = padding;
  return *this;
}

LabelStyle& LabelStyle::setAutoScale(bool autoScaleToFit,
                                     unsigned int minCharacterSize) {
  autoScaleToFit_ = autoScaleToFit;
  minCharacterSize_ = minCharacterSize;
  return *this;
}

unsigned int LabelStyle::characterSize() const { return characterSize_; }

const sf::Color& LabelStyle::color() const { return color_; }

sf::Uint32 LabelStyle::textStyle() const { return textStyle_; }

HorizontalAlign LabelStyle::horizontalAlign() const { return horizontalAlign_; }

VerticalAlign LabelStyle::verticalAlign() const { return verticalAlign_; }

const sf::Vector2f& LabelStyle::padding() const { return padding_; }

bool LabelStyle::autoScaleToFit() const { return autoScaleToFit_; }

unsigned int LabelStyle::minCharacterSize() const { return minCharacterSize_; }

ButtonStyle::ButtonStyle()
    : ButtonStyle(sf::Color::Transparent, sf::Color::Transparent,
                  sf::Color::Transparent, sf::Color::Transparent,
                  sf::Color::Transparent, 0.0f, LabelStyle()) {}

ButtonStyle::ButtonStyle(sf::Color baseColor, sf::Color hoverColor,
                         sf::Color pressedColor, sf::Color disabledColor,
                         sf::Color borderColor, float borderThickness,
                         const LabelStyle& labelStyle)
    : baseColor_(baseColor),
      hoverColor_(hoverColor),
      pressedColor_(pressedColor),
      disabledColor_(disabledColor),
      borderColor_(borderColor),
      borderThickness_(borderThickness),
      labelStyle_(labelStyle) {}

ButtonStyle& ButtonStyle::setLabelStyle(const LabelStyle& labelStyle) {
  labelStyle_ = labelStyle;
  return *this;
}

const sf::Color& ButtonStyle::baseColor() const { return baseColor_; }

const sf::Color& ButtonStyle::hoverColor() const { return hoverColor_; }

const sf::Color& ButtonStyle::pressedColor() const { return pressedColor_; }

const sf::Color& ButtonStyle::disabledColor() const { return disabledColor_; }

const sf::Color& ButtonStyle::borderColor() const { return borderColor_; }

float ButtonStyle::borderThickness() const { return borderThickness_; }

const LabelStyle& ButtonStyle::labelStyle() const { return labelStyle_; }

}  // namespace ui
