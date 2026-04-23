#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "ui/Style.hpp"

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
  enum class Layer { Background = 0, Content = 1, Overlay = 2 };

  Panel(sf::Vector2f position, sf::Vector2f size, sf::Color backgroundColor);
  Panel(sf::Vector2f position, sf::Vector2f size, const PanelStyle& style);
  ~Panel() override;

  void addChild(std::unique_ptr<Widget> child);
  void addChild(const std::string& key, std::unique_ptr<Widget> child,
                Layer layer = Layer::Content);
  Widget* getChild(const std::string& key);
  const Widget* getChild(const std::string& key) const;
  bool hasChild(const std::string& key) const;
  bool removeChild(const std::string& key);
  void clearChildren();
  void clearChildren(Layer layer);

  void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window) override;

 protected:
  void rebuildKeyIndex();

  PanelStyle style_;
  std::vector<std::tuple<std::string, Layer, std::unique_ptr<Widget>>>
      children_;
  std::unordered_map<std::string, Widget*> keyedChildren_;
  sf::RectangleShape background;
};

class Label : public Widget {
 public:
  Label(const std::string& text, const sf::Font& font,
        const LabelStyle& style = {});
  Label(const std::string& text, const sf::Font& font, sf::Vector2f position,
        sf::Vector2f size, const LabelStyle& style = {});
  ~Label() override;

  void handleEvent(sf::Event&, sf::RenderWindow&) override {}
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow&) override {}

  sf::Text& text() { return text_; }

  void setText(const std::string& text);
  void setStyle(const LabelStyle& style);
  void setRotation(float degrees);
  float rotation() const { return rotationDegrees_; }

 protected:
  void applyStyle();

  LabelStyle style_;
  sf::Text text_;
  float rotationDegrees_ = 0.0f;
};

class ImagePanel : public Widget {
 public:
  ImagePanel(const std::string& imagePath);
  ~ImagePanel() override;

  void handleEvent(sf::Event&, sf::RenderWindow&) override {}
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow&) override {}

  sf::Texture& texture() { return texture_; }
  sf::Sprite& sprite() { return sprite_; }

  void setTexture(const sf::Texture& texture) {
    texture_ = texture;
    sprite_.setTexture(texture_);
  }
  void setSprite(const sf::Sprite& sprite) { sprite_ = sprite; }

 protected:
  sf::Texture texture_;
  sf::Sprite sprite_;
};

class Button : public Widget {
 public:
  Button(const std::string& text, const sf::Font& font,
         const ButtonStyle& style);
  Button(const std::string& text, const sf::Font& font, sf::Vector2f position,
         sf::Vector2f size, const ButtonStyle& style,
         std::function<void()> onClick = {});
  Button(const std::string& text, const sf::Font& font,
         const std::string& imagePath, const ButtonStyle& style);
  Button(const std::string& text, const sf::Font& font,
         const std::string& imagePath, sf::Vector2f position, sf::Vector2f size,
         const ButtonStyle& style, std::function<void()> onClick = {});
  Button(std::unique_ptr<Widget> widget);
  ~Button() override;

  void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window) override;

  void setStyle(const ButtonStyle& style);
  void setOnClick(std::function<void()> onClick);
  void setOnHoverChanged(std::function<void(bool)> onHoverChanged);

 protected:
  bool contains(const sf::Vector2f& point) const;
  void updateHoverState(const sf::Vector2f& mousePosition);

  bool isHovered_ = false;
  bool isPressed_ = false;
  ButtonStyle style_;
  std::function<void()> onClick_;
  std::function<void(bool)> onHoverChanged_;
  sf::RectangleShape background_;
  std::unique_ptr<Widget> widget_;
};

}  // namespace ui