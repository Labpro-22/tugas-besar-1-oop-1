#pragma once

#include <SFML/System/Vector2.hpp>

namespace ui {

/**
 * @brief Value object that encapsulates tile dimensions for a board with a
 * given number of side tiles [4..14] correspond to valid board size: 5 x 5 (20)
 * up to 15 x 15 (60) tiles.
 *
 * This object is used to calculate the tile size with various options.
 */
class TileGeometry {
 public:
  explicit TileGeometry(int numTiles);

  /**
   * @brief Returns {sideWidth, sideHeight}:
   * The width parallel to the board edge,
   * The height perpendicular to board edge (depth inward).
   *
   * @return sf::Vector2f
   */
  sf::Vector2f sideSize() const;

  /**
   * @brief Dimension of a side tile parallel to its board edge.
   *
   * @return float
   */
  float sideWidth() const;

  /**
   * @brief Dimension of a side tile perpendicular to its board edge (depth).
   * Equals cornerSide() for all numTiles values.
   *
   * @return float
   */
  float sideHeight() const;

  /**
   * @brief Corner tile size
   *
   * @return sf::Vector2f
   */
  sf::Vector2f cornerSize() const;

  /**
   * @brief Corner tile length
   *
   * @return float
   */
  float cornerSide() const;

  int numTiles() const { return numTiles_; }

 private:
  int numTiles_;
};

}  // namespace ui
