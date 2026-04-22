#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

namespace ui {

enum class HorizontalAlign { Left, Center, Right };
enum class VerticalAlign { Top, Middle, Bottom };

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

  unsigned int characterSize() const;
  const sf::Color& color() const;
  sf::Uint32 textStyle() const;
  HorizontalAlign horizontalAlign() const;
  VerticalAlign verticalAlign() const;
  const sf::Vector2f& padding() const;
  bool autoScaleToFit() const;
  unsigned int minCharacterSize() const;

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

  const sf::Color& baseColor() const;
  const sf::Color& hoverColor() const;
  const sf::Color& pressedColor() const;
  const sf::Color& disabledColor() const;
  const sf::Color& borderColor() const;
  float borderThickness() const;
  const LabelStyle& labelStyle() const;

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
