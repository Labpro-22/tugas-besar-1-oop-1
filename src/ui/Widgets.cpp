#include "ui/Widgets.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <memory>
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
             unsigned int characterSize)
    : Widget(), text_(text, font, characterSize) {}

Label::~Label() = default;

void Label::render(sf::RenderWindow& window) {
  if (!isVisible()) return;

  text_.setPosition(position());
  text_.setFillColor(sf::Color::White);
  text_.setCharacterSize(24);
  text_.setString(text_.getString());
  window.draw(text_);
}

ImagePanel::ImagePanel(const std::string& imagePath) : Widget() {
  if (!texture_.loadFromFile(imagePath)) {
    throw std::runtime_error("Failed to load image: " + imagePath);
  }
  sprite_.setTexture(texture_);
}

ImagePanel::~ImagePanel() = default;

void ImagePanel::render(sf::RenderWindow& window) {
  if (!isVisible()) return;

  sprite_.setPosition(position());
  window.draw(sprite_);
}

Button::Button(const std::string& text)
    : Widget(), widget_(std::make_unique<Label>(text, sf::Font(), 24)) {
  background_.setSize(size());
  background_.setFillColor(sf::Color::Blue);
}

Button::Button(const std::string& text, const std::string& imagePath)
    : Widget(), widget_(std::make_unique<ImagePanel>(imagePath)) {
  background_.setSize(size());
  background_.setFillColor(sf::Color::Blue);
}

Button::Button(std::unique_ptr<Widget> widget)
    : Widget(), widget_(std::move(widget)) {}

Button::~Button() = default;

void Button::handleEvent(sf::Event& event, sf::RenderWindow& window) {
  if (!isActive()) return;

  if (event.type == sf::Event::MouseButtonPressed) {
    sf::Vector2f mousePos =
        window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if (background_.getGlobalBounds().contains(mousePos)) {
      // Handle button click
      std::cout << "Button clicked!\n";
    }
  }
}

void Button::render(sf::RenderWindow& window) {
  if (!isVisible()) return;

  background_.setPosition(position());
  background_.setSize(size());
  background_.setFillColor(background_.getFillColor());
  window.draw(background_);

  if (widget_) {
    widget_->setPosition(position());
    widget_->render(window);
  }
}

void Button::update(sf::RenderWindow& window) {
  if (widget_) {
    widget_->update(window);
  }
}
}  // namespace ui