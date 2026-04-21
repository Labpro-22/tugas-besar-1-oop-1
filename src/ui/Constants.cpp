
#include "ui/Constants.hpp"

#include <SFML/System/Vector2.hpp>

sf::Vector2f ui::size::baseTileSize(int numTiles) {
  switch (numTiles) {
    case 4:
      return {0.18f * boardSize, 0.16f * boardSize};
    case 5:
      return {0.15f * boardSize, 0.14f * boardSize};
    case 6:
      return {0.14f * boardSize, 0.12f * boardSize};
    case 7:
      return {0.15f * boardSize, 0.09f * boardSize};
    case 8:
      return {0.14f * boardSize, 0.08f * boardSize};
    case 9:
    default:
      return {0.14f * boardSize, 0.08f * boardSize};
    case 10:
      return {0.125f * boardSize, 0.075f * boardSize};
    case 11:
      return {0.115f * boardSize, 0.07f * boardSize};
    case 12:
      return {0.11f * boardSize, 0.065f * boardSize};
    case 13:
      return {0.11f * boardSize, 0.06f * boardSize};
    case 14:
      return {0.115f * boardSize, 0.055f * boardSize};
  }
}

float ui::size::baseTileWidth(int numTiles) { return baseTileSize(numTiles).x; }

float ui::size::tileLogoSize(int numTiles, bool isCorner) {
  return isCorner ? baseTileWidth(numTiles) : baseTileWidth(numTiles) * 0.5;
}

float ui::size::tileAccentHeight(int numTiles) {
  return baseTileWidth(numTiles) * 0.2;
}