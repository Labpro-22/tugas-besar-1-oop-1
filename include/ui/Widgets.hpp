#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <vector>

namespace ui {

class Widget {
 public:
  Widget(sf::Vector2f position = {0, 0}, sf::Vector2f size = {100, 100});
  virtual ~Widget() = default;

  virtual void handleEvent(sf::Event& event, sf::RenderWindow& window) = 0;
  virtual void render(sf::RenderWindow& window) = 0;
  virtual void update(sf::RenderWindow& window) = 0;

  void setPosition(sf::Vector2f position) { position_ = position; }
  void setSize(sf::Vector2f size) { size_ = size; }
  void setVisible(bool isVisible) { isVisible_ = isVisible; }
  void setActive(bool isActive) { isActive_ = isActive; }

  const sf::Vector2f& position() const { return position_; }
  const sf::Vector2f& size() const { return size_; }
  bool isVisible() const { return isVisible_; }
  bool isActive() const { return isActive_; }

 protected:
  sf::Vector2f position_;
  sf::Vector2f size_;
  bool isVisible_ = true;
  bool isActive_ = true;
};

class Panel : public Widget {
 public:
  Panel(sf::Vector2f position, sf::Vector2f size, sf::Color backgroundColor);
  ~Panel() override;

  void addChild(std::unique_ptr<Widget> child);
  void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window) override;

 protected:
  std::vector<std::unique_ptr<Widget>> children;
  sf::RectangleShape background;
};

class Label : public Widget {
 public:
  Label(const std::string& text, const sf::Font& font,
        unsigned int characterSize = 30);
  ~Label() override;

  void handleEvent(sf::Event& event, sf::RenderWindow& window) override {}
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window) override {}

 protected:
  sf::Text text_;
};

class ImagePanel : public Widget {
 public:
  ImagePanel(const std::string& imagePath);
  ~ImagePanel() override;

  void handleEvent(sf::Event& event, sf::RenderWindow& window) override {}
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window) override {}

 protected:
  sf::Texture texture_;
  sf::Sprite sprite_;
};

class Button : public Widget {
 public:
  Button(const std::string& text);
  Button(const std::string& text, const std::string& imagePath);
  Button(std::unique_ptr<Widget> widget);
  ~Button() override;

  void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window) override;

 protected:
  sf::RectangleShape background_;
  std::unique_ptr<Widget> widget_;
};

}  // namespace ui