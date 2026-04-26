#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>

namespace ui {

class AssetsManager {
 public:
  AssetsManager(const AssetsManager&) = delete;
  AssetsManager& operator=(const AssetsManager&) = delete;

  AssetsManager(AssetsManager&&) = delete;

  AssetsManager& operator=(AssetsManager&&) = delete;

  static AssetsManager& get() {
    static AssetsManager instance;
    return instance;
  }

  sf::Texture& getTexture(const std::string& name) {
    return getAsset(name, textures_);
  }

  sf::Font& getFont(const std::string& name) { return getAsset(name, fonts_); }

 private:
  AssetsManager() = default;

  template <typename T>
  T& getAsset(const std::string& name,
              std::unordered_map<std::string, std::unique_ptr<T>>& container) {
    auto it = container.find(name);
    if (it != container.end()) {
      return *(it->second);
    }

    auto asset = std::make_unique<T>();
    if (!asset->loadFromFile(name)) {
      throw std::runtime_error(
          std::string("Unable to load asset from ").append(name));
    }

    auto& ref = *asset;
    container[name] = std::move(asset);
    return ref;
  }
  std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures_;
  std::unordered_map<std::string, std::unique_ptr<sf::Font>> fonts_;
};
}  // namespace ui