
#include "ui/Constants.hpp"

#include <SFML/System/Vector2.hpp>

sf::Vector2f ui::size::baseTileSize(int numTiles) {
  switch (numTiles) {
    case 4:
      return {0.16f * boardSize, 0.18f * boardSize};
    case 5:
      return {0.14f * boardSize, 0.15f * boardSize};
    case 6:
      return {0.12f * boardSize, 0.14f * boardSize};
    case 7:
      return {0.10f * boardSize, 0.15f * boardSize};
    case 8:
      return {0.09f * boardSize, 0.14f * boardSize};
    case 9:
    default:
      return {0.08f * boardSize, 0.14f * boardSize};
    case 10:
      return {0.075f * boardSize, 0.125f * boardSize};
    case 11:
      return {0.07f * boardSize, 0.115f * boardSize};
    case 12:
      return {0.065f * boardSize, 0.11f * boardSize};
    case 13:
      return {0.06f * boardSize, 0.11f * boardSize};
    case 14:
      return {0.055f * boardSize, 0.115f * boardSize};
  }
}

float ui::size::regularTileWidth(int numTiles) {
  return baseTileSize(numTiles).x;
}

float ui::size::cornerTileSize(int numTiles) {
  return baseTileSize(numTiles).y;
}

float ui::size::tileLogoSize(int numTiles, bool isCorner) {
  return isCorner ? cornerTileSize(numTiles) * 0.75f
                  : regularTileWidth(numTiles) * 0.5f;
}

float ui::size::tileAccentHeight(int numTiles) {
  return cornerTileSize(numTiles) * 0.2f;
}