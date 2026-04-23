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

using ChildEntry =
    std::tuple<std::string, Panel::Layer, std::unique_ptr<Widget>>;

Panel::Panel(sf::Vector2f position, sf::Vector2f size,
             sf::Color backgroundColor)
    : Widget(position, size), style_(backgroundColor), background(size) {}

Panel::Panel(sf::Vector2f position, sf::Vector2f size, const PanelStyle& style)
    : Widget(position, size), style_(style), background(size) {}

Panel::~Panel() = default;

void Panel::addChild(std::unique_ptr<Widget> child) {
  addChild("", std::move(child), Layer::Content);
}

void Panel::addChild(const std::string& key, std::unique_ptr<Widget> child,
                     Layer layer) {
  if (!child) {
    return;
  }

  if (!key.empty()) {
    removeChild(key);
  }

  Widget* childPtr = child.get();
  children_.push_back(std::make_tuple(key, layer, std::move(child)));
  if (!key.empty()) {
    keyedChildren_[key] = childPtr;
  }
}

Widget* Panel::getChild(const std::string& key) {
  const auto it = keyedChildren_.find(key);
  return it == keyedChildren_.end() ? nullptr : it->second;
}

const Widget* Panel::getChild(const std::string& key) const {
  const auto it = keyedChildren_.find(key);
  return it == keyedChildren_.end() ? nullptr : it->second;
}

bool Panel::hasChild(const std::string& key) const {
  return keyedChildren_.find(key) != keyedChildren_.end();
}

bool Panel::removeChild(const std::string& key) {
  if (key.empty()) {
    return false;
  }

  const auto initialSize = children_.size();
  children_.erase(std::remove_if(children_.begin(), children_.end(),
                                 [&](const ChildEntry& entry) {
                                   return std::get<0>(entry) == key;
                                 }),
                  children_.end());

  if (children_.size() == initialSize) {
    return false;
  }

  rebuildKeyIndex();
  return true;
}

void Panel::clearChildren() {
  children_.clear();
  keyedChildren_.clear();
}

void Panel::clearChildren(Layer layer) {
  children_.erase(std::remove_if(children_.begin(), children_.end(),
                                 [&](const ChildEntry& entry) {
                                   return std::get<1>(entry) == layer;
                                 }),
                  children_.end());
  rebuildKeyIndex();
}

void Panel::rebuildKeyIndex() {
  keyedChildren_.clear();
  for (const auto& entry : children_) {
    if (!std::get<0>(entry).empty() && std::get<2>(entry)) {
      keyedChildren_[std::get<0>(entry)] = std::get<2>(entry).get();
    }
  }
}

void Panel::handleEvent(sf::Event& event, sf::RenderWindow& window) {
  const Panel::Layer eventOrder[] = {Layer::Overlay, Layer::Content,
                                     Layer::Background};
  for (Panel::Layer layer : eventOrder) {
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
      if (std::get<1>(*it) != layer || !std::get<2>(*it) ||
          !std::get<2>(*it)->isActive()) {
        continue;
      }
      std::get<2>(*it)->handleEvent(event, window);
    }
  }
}

void Panel::render(sf::RenderWindow& window) {
  if (!isVisible()) return;

  background.setPosition(position());
  background.setSize(size());
  background.setFillColor(style_.backgroundColor());
  background.setOutlineColor(style_.borderColor());
  background.setOutlineThickness(style_.borderThickness());
  window.draw(background);

  const Panel::Layer renderOrder[] = {Layer::Background, Layer::Content,
                                      Layer::Overlay};
  for (Panel::Layer layer : renderOrder) {
    for (auto& child : children_) {
      if (std::get<1>(child) != layer || !std::get<2>(child) ||
          !std::get<2>(child)->isVisible()) {
        continue;
      }
      std::get<2>(child)->render(window);
    }
  }
}

void Panel::update(sf::RenderWindow& window) {
  const Panel::Layer updateOrder[] = {Layer::Background, Layer::Content,
                                      Layer::Overlay};
  for (Panel::Layer layer : updateOrder) {
    for (auto& child : children_) {
      if (std::get<1>(child) != layer || !std::get<2>(child) ||
          !std::get<2>(child)->isActive()) {
        continue;
      }
      std::get<2>(child)->update(window);
    }
  }
}

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