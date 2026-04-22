#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
namespace ui::size {

const float multiplier = 120; /**< Choose 120 for FHD, 80 for HD */
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

namespace ui::font {

inline constexpr const char* primaryFamily =
    "assets/fonts/ITCKabelStdMedium.ttf";
inline constexpr const char* secondaryFamily =
    "assets/fonts/Helvetica-Condensed.ttf";
}  // namespace ui::font

namespace ui::typography {

const float scale = size::multiplier / 80.0f;

const unsigned int title = static_cast<unsigned int>(24.0f * scale);
const unsigned int sectionTitle = static_cast<unsigned int>(18.0f * scale);
const unsigned int buttonPrimary = static_cast<unsigned int>(14.0f * scale);
const unsigned int buttonSecondary = static_cast<unsigned int>(12.0f * scale);
const unsigned int infoTitle = static_cast<unsigned int>(13.0f * scale);
const unsigned int infoBody = static_cast<unsigned int>(12.0f * scale);
const unsigned int logTitle = static_cast<unsigned int>(12.0f * scale);
const unsigned int logBody = static_cast<unsigned int>(11.0f * scale);

const unsigned int playerCardTitle = static_cast<unsigned int>(18.0f * scale);
const unsigned int playerCardBody = static_cast<unsigned int>(13.0f * scale);
const unsigned int boardInfo = static_cast<unsigned int>(12.0f * scale);
const unsigned int dialogTitle = static_cast<unsigned int>(22.0f * scale);
const unsigned int dialogBody = static_cast<unsigned int>(14.0f * scale);

const sf::Uint32 titleStyle = sf::Text::Bold;
const sf::Uint32 sectionStyle = sf::Text::Bold;
const sf::Uint32 buttonStyle = sf::Text::Bold;
const sf::Uint32 infoTitleStyle = sf::Text::Bold | sf::Text::Italic;
const sf::Uint32 infoBodyStyle = sf::Text::Regular;
const sf::Uint32 logStyle = sf::Text::Italic;

}  // namespace ui::typography

namespace ui::layout::actionPanel {

const float outerPadding = 0.1f * size::multiplier;
const float sectionGap = 0.08f * size::multiplier;
const float rowGap = 0.05f * size::multiplier;
const float innerPadding = 0.08f * size::multiplier;

const float titleHeight = 0.42f * size::multiplier;
const float topActionHeight = 0.34f * size::multiplier;
const float tileInfoHeight = 0.72f * size::multiplier;
const float tileActionHeight = 0.30f * size::multiplier;
const float logHeaderHeight = 0.33f * size::multiplier;
const float logBodyHeight = 4 * size::multiplier;
const float gameOptionHeight = 0.36f * size::multiplier;

const float headerButtonWidth = 0.95f * size::multiplier;
const float borderThickness = 0.016f * size::multiplier;
const float textPaddingX = 0.10f * size::multiplier;
const float textPaddingY = 0.06f * size::multiplier;

}  // namespace ui::layout::actionPanel