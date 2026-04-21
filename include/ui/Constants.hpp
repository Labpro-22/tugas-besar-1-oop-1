#pragma once

#include <SFML/System/Vector2.hpp>
namespace ui::size {

const float multiplier = 80; /**< Choose 120 for FHD, 80 for HD */
const float width = 16 * multiplier;
const float height = 9 * multiplier;
const float sideWidth = 3.5f * multiplier;
const float defaultMargin = 0.25 * multiplier;

const float playerCardWidth = 3 * multiplier;
const float playerCardHeight = 1.75 * multiplier;
const float playerCardLogo = 0.75 * multiplier;

const float boardSize = 8.5 * multiplier;
const float lineThickness = 0.025 * multiplier;

const float boardCardWidth = 1 * multiplier;
const float boardCardHeight = 1.5 * multiplier;
const float middleCircleRadius = 1 * multiplier;

const float buttonPanelWidth = 3 * multiplier;
const float buttonPanelHeight = 1.5 * multiplier;
const float textBoxHeight = 0.5 * multiplier;

/**
 * @brief Calculates the base size of a tile based on the number of tiles
 * excluding corners. The size of the board starts from 4 (4 x 4 + 4 = 20 tiles)
 * to 14 (14 x 4 + 4 = 60 tiles). The default is 9 (9 x 4 + 4 = 40 tiles).
 *
 * @param numTiles
 * @return std::pair<float, float>
 */
sf::Vector2f baseTileSize(int numTiles = 9);

/**
 * @brief Get the base tile only from `baseTileSize()`
 *
 * @param numTiles
 * @return float
 */
float baseTileWidth(int numTiles = 9);

/**
 * @brief Return the tile logo size relative to the current tile size.
 *
 * @param numTiles
 * @return float
 */
float tileLogoSize(int numTiles = 9, bool isCorner = false);

/**
 * @brief Return the tile accent height for street tile relative to the current
 * tile size
 *
 * @param numTiles
 * @return float
 */
float tileAccentHeight(int numTiles = 9);

}  // namespace ui::size