#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

namespace ui {

enum class HorizontalAlign { Left, Center, Right };
enum class VerticalAlign { Top, Middle, Bottom };

class PanelStyle {
 public:
  PanelStyle() = default;
  PanelStyle(sf::Color backgroundColor);
  PanelStyle(sf::Color backgroundColor, sf::Color borderColor,
             float borderThickness);
  PanelStyle(sf::Color backgroundColor, sf::Color hoverColor,
             sf::Color pressedColor, sf::Color disabledColor,
             sf::Color borderColor, float borderThickness);

  PanelStyle& setBackgroundColor(sf::Color backgroundColor);
  PanelStyle& setHoverColor(sf::Color hoverColor);
  PanelStyle& setPressedColor(sf::Color pressedColor);
  PanelStyle& setDisabledColor(sf::Color disabledColor);
  PanelStyle& setBorderColor(sf::Color borderColor);
  PanelStyle& setBorderThickness(float borderThickness);

  const sf::Color& backgroundColor() const { return backgroundColor_; }
  const sf::Color& hoverColor() const { return hoverColor_; }
  const sf::Color& pressedColor() const { return pressedColor_; }
  const sf::Color& disabledColor() const { return disabledColor_; }
  const sf::Color& borderColor() const { return borderColor_; }
  float borderThickness() const { return borderThickness_; }

 private:
  sf::Color backgroundColor_ = sf::Color::Transparent;
  sf::Color hoverColor_ = backgroundColor_;
  sf::Color pressedColor_ = backgroundColor_;
  sf::Color disabledColor_ = backgroundColor_;
  sf::Color borderColor_ = sf::Color::Transparent;
  float borderThickness_ = 0.0f;
};

class LabelStyle {
 public:
  LabelStyle();
  LabelStyle(unsigned int characterSize, sf::Color color,
             sf::Uint32 textStyle = sf::Text::Regular,
             HorizontalAlign horizontalAlign = HorizontalAlign::Left,
             VerticalAlign verticalAlign = VerticalAlign::Top,
             sf::Vector2f padding = {0.0f, 0.0f}, bool autoScaleToFit = false,
             unsigned int minCharacterSize = 10);

  LabelStyle& setAlignment(HorizontalAlign horizontalAlign,
                           VerticalAlign verticalAlign);
  LabelStyle& setPadding(sf::Vector2f padding);
  LabelStyle& setAutoScale(bool autoScaleToFit,
                           unsigned int minCharacterSize = 10);

  unsigned int characterSize() const { return characterSize_; }
  const sf::Color& color() const { return color_; }
  sf::Uint32 textStyle() const { return textStyle_; }
  HorizontalAlign horizontalAlign() const { return horizontalAlign_; }
  VerticalAlign verticalAlign() const { return verticalAlign_; }
  const sf::Vector2f& padding() const { return padding_; }
  bool autoScaleToFit() const { return autoScaleToFit_; }
  unsigned int minCharacterSize() const { return minCharacterSize_; }

 private:
  unsigned int characterSize_;
  sf::Color color_;
  sf::Uint32 textStyle_;
  HorizontalAlign horizontalAlign_;
  VerticalAlign verticalAlign_;
  sf::Vector2f padding_;
  bool autoScaleToFit_;
  unsigned int minCharacterSize_;
};

class ButtonStyle {
 public:
  ButtonStyle();
  ButtonStyle(sf::Color baseColor, sf::Color hoverColor, sf::Color pressedColor,
              sf::Color disabledColor = sf::Color::Transparent,
              sf::Color borderColor = sf::Color::Transparent,
              float borderThickness = 0.0f,
              const LabelStyle& labelStyle = LabelStyle());

  ButtonStyle& setLabelStyle(const LabelStyle& labelStyle);

  const sf::Color& baseColor() const { return baseColor_; }
  const sf::Color& hoverColor() const { return hoverColor_; }
  const sf::Color& pressedColor() const { return pressedColor_; }
  const sf::Color& disabledColor() const { return disabledColor_; }
  const sf::Color& borderColor() const { return borderColor_; }
  float borderThickness() const { return borderThickness_; }
  const LabelStyle& labelStyle() const { return labelStyle_; }

 private:
  sf::Color baseColor_;
  sf::Color hoverColor_;
  sf::Color pressedColor_;
  sf::Color disabledColor_;
  sf::Color borderColor_;
  float borderThickness_;
  LabelStyle labelStyle_;
};

}  // namespace ui
