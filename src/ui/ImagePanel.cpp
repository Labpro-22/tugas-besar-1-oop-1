#include <SFML/Graphics/Texture.hpp>
#include <string>

#include "ui/AssetsManager.hpp"
#include "ui/Widgets.hpp"

namespace ui {

ImagePanel::ImagePanel(sf::Vector2f position, sf::Vector2f size,
                       const std::string& imagePath)
    : Widget(position, size),
      texture_(AssetsManager::get().getTexture(imagePath)) {
  sprite_.setTexture(texture_);

  sf::Vector2u texSize = texture_.getSize();
  float tw = static_cast<float>(texSize.x);
  float th = static_cast<float>(texSize.y);

  float scaleX = size.x / tw;
  float scaleY = size.y / th;
  float uniformScale = std::min(scaleX, scaleY);
  sprite_.setScale(uniformScale, uniformScale);

  sprite_.setOrigin(tw / 2.f, th / 2.f);
  sprite_.setPosition(position.x + (size.x / 2.f), position.y + (size.y / 2.f));
}

ImagePanel::~ImagePanel() = default;

void ImagePanel::render(sf::RenderWindow& window) {
  if (!isVisible()) return;

  sprite_.setRotation(rotationDegrees_);
  window.draw(sprite_);
}

}  // namespace ui
