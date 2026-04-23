#include <SFML/Graphics/Texture.hpp>
#include <stdexcept>
#include <string>

#include "ui/Widgets.hpp"

namespace ui {

ImagePanel::ImagePanel(const std::string& imagePath) : Widget() {
  if (!texture_.loadFromFile(imagePath)) {
    throw std::runtime_error("Failed to load image: " + imagePath);
  }
  sprite_.setTexture(texture_);
  size_.x = static_cast<float>(sprite_.getTexture()->getSize().x);
  size_.y = static_cast<float>(sprite_.getTexture()->getSize().y);
}

ImagePanel::~ImagePanel() = default;

void ImagePanel::render(sf::RenderWindow& window) {
  if (!isVisible()) return;

  sprite_.setPosition(position());
  window.draw(sprite_);
}

}  // namespace ui
