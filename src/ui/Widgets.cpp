#include "ui/Widgets.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <algorithm>
#include <cmath>
#include <memory>
#include <stdexcept>
#include <string>

namespace ui {
Widget::Widget(sf::Vector2f position, sf::Vector2f size)
    : position_(position), size_(size) {}

Panel::Panel(sf::Vector2f position, sf::Vector2f size,
             sf::Color backgroundColor)
    : Widget(position, size), background(size) {
  background.setFillColor(backgroundColor);
}

Panel::~Panel() = default;

void Panel::addChild(std::unique_ptr<Widget> child) {
  children.push_back(std::move(child));
}

void Panel::handleEvent(sf::Event& event, sf::RenderWindow& window) {
  for (auto& child : children) {
    if (child->isActive()) {
      child->handleEvent(event, window);
    }
  }
}

void Panel::render(sf::RenderWindow& window) {
  if (!isVisible()) return;

  background.setPosition(position());
  background.setSize(size());
  background.setFillColor(background.getFillColor());
  window.draw(background);

  for (auto& child : children) {
    if (child->isVisible()) {
      child->render(window);
    }
  }
}

void Panel::update(sf::RenderWindow& window) {
  for (auto& child : children) {
    if (child->isActive()) {
      child->update(window);
    }
  }
}

Label::Label(const std::string& text, const sf::Font& font,
             const LabelStyle& style)
    : Widget(), style_(style), text_(text, font, style.characterSize) {
  applyStyle();
}

Label::~Label() = default;

void Label::setText(const std::string& text) { text_.setString(text); }

void Label::setStyle(const LabelStyle& style) {
  style_ = style;
  applyStyle();
}

void Label::applyStyle() {
  text_.setFillColor(style_.color);
  text_.setStyle(style_.textStyle);
  text_.setCharacterSize(style_.characterSize);
}

void Label::render(sf::RenderWindow& window) {
  if (!isVisible()) return;

  applyStyle();
  auto textBounds = text_.getLocalBounds();

  if (style_.autoScaleToFit && size().x > 0.0f) {
    const float maxWidth = std::max(0.0f, size().x - (2.0f * style_.padding.x));
    if (textBounds.width > maxWidth && textBounds.width > 0.0f) {
      const float scale = maxWidth / textBounds.width;
      const float candidate =
          std::floor(static_cast<float>(style_.characterSize) * scale);
      const auto fittedSize = static_cast<unsigned int>(
          std::max(static_cast<float>(style_.minCharacterSize), candidate));
      text_.setCharacterSize(fittedSize);
      textBounds = text_.getLocalBounds();
    }
  }

  float posX = position().x - textBounds.left;
  float posY = position().y - textBounds.top;

  if (size().x > 0.0f) {
    switch (style_.horizontalAlign) {
      case HorizontalAlign::Left:
        posX = position().x + style_.padding.x - textBounds.left;
        break;
      case HorizontalAlign::Center:
        posX = position().x + ((size().x - textBounds.width) * 0.5f) -
               textBounds.left;
        break;
      case HorizontalAlign::Right:
        posX = position().x + size().x - style_.padding.x - textBounds.width -
               textBounds.left;
        break;
    }
  }

  if (size().y > 0.0f) {
    switch (style_.verticalAlign) {
      case VerticalAlign::Top:
        posY = position().y + style_.padding.y - textBounds.top;
        break;
      case VerticalAlign::Middle:
        posY = position().y + ((size().y - textBounds.height) * 0.5f) -
               textBounds.top;
        break;
      case VerticalAlign::Bottom:
        posY = position().y + size().y - style_.padding.y - textBounds.height -
               textBounds.top;
        break;
    }
  }

  text_.setPosition(posX, posY);
  window.draw(text_);
}

ImagePanel::ImagePanel(const std::string& imagePath) : Widget() {
  if (!texture_.loadFromFile(imagePath)) {
    throw std::runtime_error("Failed to load image: " + imagePath);
  }
  sprite_.setTexture(texture_);
  size_.x = sprite_.getTexture()->getSize().x;
  size_.y = sprite_.getTexture()->getSize().y;
}

ImagePanel::~ImagePanel() = default;

void ImagePanel::render(sf::RenderWindow& window) {
  if (!isVisible()) return;

  sprite_.setPosition(position());
  window.draw(sprite_);
}

Button::Button(const std::string& text, const sf::Font& font,
               const ButtonStyle& style)
    : Widget(),
      isHovered_(false),
      isPressed_(false),
      style_(style),
      widget_(std::make_unique<Label>(text, font, style.labelStyle)) {
  background_.setSize(size());
  background_.setFillColor(style_.baseColor);
}

Button::Button(const std::string& text, const sf::Font& font,
               const std::string& imagePath, const ButtonStyle& style)
    : Widget(),
      isHovered_(false),
      isPressed_(false),
      style_(style),
      widget_(std::make_unique<ImagePanel>(imagePath)) {
  (void)text;
  (void)font;
  background_.setSize(size());
  background_.setFillColor(style_.baseColor);
}

Button::Button(std::unique_ptr<Widget> widget)
    : Widget(), widget_(std::move(widget)) {}

Button::~Button() = default;

void Button::handleEvent(sf::Event& event, sf::RenderWindow& window) {
  const sf::Vector2f mousePos =
      window.mapPixelToCoords(sf::Mouse::getPosition(window));
  updateHoverState(mousePos);

  if (!isActive()) {
    isPressed_ = false;
    return;
  }

  if (event.type == sf::Event::MouseButtonPressed &&
      event.mouseButton.button == sf::Mouse::Left) {
    isPressed_ = isHovered_;
  }

  if (event.type == sf::Event::MouseButtonReleased &&
      event.mouseButton.button == sf::Mouse::Left) {
    const bool shouldClick = isPressed_ && isHovered_;
    isPressed_ = false;
    if (shouldClick && onClick_) {
      onClick_();
    }
  }
}

void Button::render(sf::RenderWindow& window) {
  if (!isVisible()) return;

  background_.setPosition(position());
  background_.setSize(size());
  background_.setOutlineColor(style_.borderColor);
  background_.setOutlineThickness(style_.borderThickness);

  if (!isActive()) {
    background_.setFillColor(style_.disabledColor);
  } else if (isPressed_) {
    background_.setFillColor(style_.pressedColor);
  } else if (isHovered_) {
    background_.setFillColor(style_.hoverColor);
  } else {
    background_.setFillColor(style_.baseColor);
  }

  window.draw(background_);

  if (widget_) {
    widget_->setPosition(position());
    widget_->setSize(size());
    widget_->render(window);
  }
}

void Button::update(sf::RenderWindow& window) {
  updateHoverState(window.mapPixelToCoords(sf::Mouse::getPosition(window)));

  if (widget_) {
    widget_->update(window);
  }
}

void Button::setStyle(const ButtonStyle& style) {
  style_ = style;
  if (auto* label = dynamic_cast<Label*>(widget_.get())) {
    label->setStyle(style_.labelStyle);
  }
}

void Button::setOnClick(std::function<void()> onClick) {
  onClick_ = std::move(onClick);
}

void Button::setOnHoverChanged(std::function<void(bool)> onHoverChanged) {
  onHoverChanged_ = std::move(onHoverChanged);
}

bool Button::contains(const sf::Vector2f& point) const {
  return sf::FloatRect(position(), size()).contains(point);
}

void Button::updateHoverState(const sf::Vector2f& mousePosition) {
  const bool nowHovered = contains(mousePosition);
  if (nowHovered == isHovered_) {
    return;
  }

  isHovered_ = nowHovered;
  if (onHoverChanged_) {
    onHoverChanged_(isHovered_);
  }
}
}  // namespace ui