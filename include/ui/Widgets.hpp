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
#include <vector>

namespace ui {

enum class HorizontalAlign { Left, Center, Right };
enum class VerticalAlign { Top, Middle, Bottom };

struct LabelStyle {
  unsigned int characterSize = 30;
  sf::Color color = sf::Color::White;
  sf::Uint32 textStyle = sf::Text::Regular;
  HorizontalAlign horizontalAlign = HorizontalAlign::Left;
  VerticalAlign verticalAlign = VerticalAlign::Top;
  sf::Vector2f padding{0.0f, 0.0f};
  bool autoScaleToFit = false;
  unsigned int minCharacterSize = 10;
};

struct ButtonStyle {
  sf::Color baseColor = sf::Color::Transparent;
  sf::Color hoverColor = sf::Color::Transparent;
  sf::Color pressedColor = sf::Color::Transparent;
  sf::Color disabledColor = sf::Color::Transparent;
  sf::Color borderColor = sf::Color::Transparent;
  float borderThickness = 0.0f;
  LabelStyle labelStyle;
};

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
        const LabelStyle& style = {});
  ~Label() override;

  void handleEvent(sf::Event&, sf::RenderWindow&) override {}
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow&) override {}

  void setText(const std::string& text);
  void setStyle(const LabelStyle& style);

 protected:
  void applyStyle();

  LabelStyle style_;
  sf::Text text_;
};

class ImagePanel : public Widget {
 public:
  ImagePanel(const std::string& imagePath);
  ~ImagePanel() override;

  void handleEvent(sf::Event&, sf::RenderWindow&) override {}
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow&) override {}

 protected:
  sf::Texture texture_;
  sf::Sprite sprite_;
};

class Button : public Widget {
 public:
  Button(const std::string& text, const sf::Font& font,
         const ButtonStyle& style);
  Button(const std::string& text, const sf::Font& font,
         const std::string& imagePath, const ButtonStyle& style);
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