#include <SFML/Window/Mouse.hpp>
#include <functional>
#include <memory>

#include "ui/Widgets.hpp"

namespace ui {

Button::Button(const std::string& text, const sf::Font& font,
               const ButtonStyle& style)
    : Widget(),
      isHovered_(false),
      isPressed_(false),
      style_(style),
      widget_(std::make_unique<Label>(text, font, style.labelStyle())) {
  background_.setSize(size());
  background_.setFillColor(style_.baseColor());
}

Button::Button(const std::string& text, const sf::Font& font,
               sf::Vector2f position, sf::Vector2f size,
               const ButtonStyle& style, std::function<void()> onClick)
    : Widget(position, size),
      isHovered_(false),
      isPressed_(false),
      style_(style),
      onClick_(std::move(onClick)),
      widget_(std::make_unique<Label>(text, font, position, size,
                                      style.labelStyle())) {
  background_.setSize(size);
  background_.setFillColor(style_.baseColor());
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
  background_.setFillColor(style_.baseColor());
}

Button::Button(const std::string& text, const sf::Font& font,
               const std::string& imagePath, sf::Vector2f position,
               sf::Vector2f size, const ButtonStyle& style,
               std::function<void()> onClick)
    : Widget(position, size),
      isHovered_(false),
      isPressed_(false),
      style_(style),
      onClick_(std::move(onClick)),
      widget_(std::make_unique<ImagePanel>(imagePath)) {
  (void)text;
  (void)font;
  background_.setSize(size);
  background_.setFillColor(style_.baseColor());
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
  background_.setOutlineColor(style_.borderColor());
  background_.setOutlineThickness(style_.borderThickness());

  if (!isActive()) {
    background_.setFillColor(style_.disabledColor());
  } else if (isPressed_) {
    background_.setFillColor(style_.pressedColor());
  } else if (isHovered_) {
    background_.setFillColor(style_.hoverColor());
  } else {
    background_.setFillColor(style_.baseColor());
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
    label->setStyle(style_.labelStyle());
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
