#include "ui/component/Style.hpp"

namespace ui {

PanelStyle::PanelStyle(sf::Color backgroundColor)
    : PanelStyle(backgroundColor, backgroundColor, backgroundColor,
                 backgroundColor, sf::Color::Transparent, 0.0f) {}

PanelStyle::PanelStyle(sf::Color backgroundColor, sf::Color borderColor,
                       float borderThickness)
    : PanelStyle(backgroundColor, backgroundColor, backgroundColor,
                 backgroundColor, borderColor, borderThickness) {}

PanelStyle::PanelStyle(sf::Color backgroundColor, sf::Color hoverColor,
                       sf::Color pressedColor, sf::Color disabledColor,
                       sf::Color borderColor, float borderThickness)
    : backgroundColor_(backgroundColor),
      hoverColor_(hoverColor),
      pressedColor_(pressedColor),
      disabledColor_(disabledColor),
      borderColor_(borderColor),
      borderThickness_(borderThickness) {}

PanelStyle& PanelStyle::setBackgroundColor(sf::Color backgroundColor) {
  backgroundColor_ = backgroundColor;
  return *this;
}

PanelStyle& PanelStyle::setHoverColor(sf::Color hoverColor) {
  hoverColor_ = hoverColor;
  return *this;
}

PanelStyle& PanelStyle::setPressedColor(sf::Color pressedColor) {
  pressedColor_ = pressedColor;
  return *this;
}

PanelStyle& PanelStyle::setDisabledColor(sf::Color disabledColor) {
  disabledColor_ = disabledColor;
  return *this;
}

PanelStyle& PanelStyle::setBorderColor(sf::Color borderColor) {
  borderColor_ = borderColor;
  return *this;
}

PanelStyle& PanelStyle::setBorderThickness(float borderThickness) {
  borderThickness_ = borderThickness;
  return *this;
}

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

}  // namespace ui
