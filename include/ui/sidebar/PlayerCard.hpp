#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <vector>

#include "ui/component/Widgets.hpp"
#include "ui/sidebar/Game_temp.hpp"

namespace ui {

class PropertyWidget : public Widget {
 public:
  explicit PropertyWidget(sf::Font& font);
  ~PropertyWidget() override = default;

  void handleEvent(sf::Event&, sf::RenderWindow&) override {}
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow&) override {}

  void setProperty(const temp::PropertyTuple& property);
  void setOverflow(int remainingCount);
  void applyLayout(sf::Vector2f position, sf::Vector2f size,
                   float iconHeightRatio);

 private:
  sf::Font& font_;
  sf::RectangleShape box_;
  sf::Text text_;
  sf::Sprite icon_;
  sf::Texture* iconTexture_ = nullptr;

  bool isOverflow_ = false;
  int overflowCount_ = 0;
  bool hasIcon_ = false;
  float iconHeightRatio_ = 0.35f;
  sf::Color boxColor_;
};

class PlayerCard : public Widget {
 public:
  PlayerCard(sf::Vector2f position, sf::Vector2f size);
  ~PlayerCard() override = default;

  void handleEvent(sf::Event&, sf::RenderWindow&) override {}
  void render(sf::RenderWindow& window) override;
  void update(sf::RenderWindow&) override {}

  void setPlayer(const temp::Player& player);

 private:
  void updateLayout();
  void rebuildPropertyGrid(const std::vector<temp::PropertyTuple>& properties);
  static std::string formatBalance(long long balance);

  sf::RectangleShape background_;
  sf::RectangleShape avatarBackground_;
  sf::Sprite avatarSprite_;
  sf::Texture* avatarTexture_ = nullptr;

  sf::RectangleShape namePlateBackground_;
  sf::Text nameText_;

  sf::RectangleShape turnBoxBackground_;
  sf::Text turnText_;
  sf::Sprite starSprite_;
  sf::Texture* starTexture_ = nullptr;
  bool showStar_ = false;

  sf::RectangleShape divider_;

  sf::Text balanceText_;
  sf::RectangleShape locationBoxBackground_;
  sf::Sprite mapPinSprite_;
  sf::Texture* mapPinBlackTexture_ = nullptr;
  sf::Texture* mapPinWhiteTexture_ = nullptr;
  sf::Text locationText_;

  sf::Font& secondaryFont_;
  const temp::Player* player_ = nullptr;
  std::vector<PropertyWidget> propertyWidgets_;
};

}  // namespace ui
