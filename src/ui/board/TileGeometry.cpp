#include "ui/board/TileGeometry.hpp"

#include "ui/component/Constants.hpp"

namespace ui {

TileGeometry::TileGeometry(int numTiles) : numTiles_(numTiles) {}

sf::Vector2f TileGeometry::sideSize() const {
  switch (numTiles_) {
    case 4:
      return {0.16f * size::boardSize, 0.18f * size::boardSize};
    case 5:
      return {0.14f * size::boardSize, 0.15f * size::boardSize};
    case 6:
      return {0.12f * size::boardSize, 0.14f * size::boardSize};
    case 7:
      return {0.10f * size::boardSize, 0.15f * size::boardSize};
    case 8:
      return {0.09f * size::boardSize, 0.14f * size::boardSize};
    case 9:
    default:
      return {0.08f * size::boardSize, 0.14f * size::boardSize};
    case 10:
      return {0.075f * size::boardSize, 0.125f * size::boardSize};
    case 11:
      return {0.07f * size::boardSize, 0.115f * size::boardSize};
    case 12:
      return {0.065f * size::boardSize, 0.11f * size::boardSize};
    case 13:
      return {0.06f * size::boardSize, 0.11f * size::boardSize};
    case 14:
      return {0.055f * size::boardSize, 0.115f * size::boardSize};
  }
}

sf::Vector2f TileGeometry::cornerSize() const {
  switch (numTiles_) {
    case 4:
      return {0.18f * size::boardSize, 0.18f * size::boardSize};
    case 5:
      return {0.15f * size::boardSize, 0.15f * size::boardSize};
    case 6:
      return {0.14f * size::boardSize, 0.14f * size::boardSize};
    case 7:
      return {0.15f * size::boardSize, 0.15f * size::boardSize};
    case 8:
      return {0.14f * size::boardSize, 0.14f * size::boardSize};
    case 9:
    default:
      return {0.14f * size::boardSize, 0.14f * size::boardSize};
    case 10:
      return {0.125f * size::boardSize, 0.125f * size::boardSize};
    case 11:
      return {0.115f * size::boardSize, 0.115f * size::boardSize};
    case 12:
      return {0.11f * size::boardSize, 0.11f * size::boardSize};
    case 13:
      return {0.11f * size::boardSize, 0.11f * size::boardSize};
    case 14:
      return {0.115f * size::boardSize, 0.115f * size::boardSize};
  }
}

float TileGeometry::sideWidth() const { return sideSize().x; }

float TileGeometry::sideHeight() const { return sideSize().y; }

float TileGeometry::cornerSide() const { return cornerSize().x; }

}  // namespace ui
