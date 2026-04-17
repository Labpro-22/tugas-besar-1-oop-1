#pragma once

#include <SFML/Config.hpp>
#include <SFML/Graphics/Color.hpp>

namespace ui::color::alt {
// Alternative Pallete Colorful [DO NOT CHANGE]
const auto bubblegumPink = sf::Color(sf::Uint32(0x06D6A0));
const auto goldenPollen = sf::Color(sf::Uint32(0x06D6A0));
const auto emerald = sf::Color(sf::Uint32(0x06D6A0));
const auto oceanBlue = sf::Color(sf::Uint32(0x06D6A0));
const auto darkTeal = sf::Color(sf::Uint32(0x06D6A0));
}  // namespace ui::color::alt

namespace ui::color::light::surface {
// Light mode core surfaces
const auto background = sf::Color(sf::Uint32(0xF4F6F8));
const auto gameBoardBase = sf::Color(sf::Uint32(0xE8F5E9));
const auto boardBorder = sf::Color(sf::Uint32(0xD0D5DD));
const auto gridLines = sf::Color(sf::Uint32(0xE4E7EC));
}  // namespace ui::color::light::surface

namespace ui::color::light::property {
// Light mode property tile colors
const auto brown = sf::Color(sf::Uint32(0x8B5A2B));
const auto lightBlue = sf::Color(sf::Uint32(0x6EC1E4));
const auto pink = sf::Color(sf::Uint32(0xE56B9F));
const auto orange = sf::Color(sf::Uint32(0xF59E0B));
const auto red = sf::Color(sf::Uint32(0xEF4444));
const auto yellow = sf::Color(sf::Uint32(0xFACC15));
const auto green = sf::Color(sf::Uint32(0x22C55E));
const auto darkBlue = sf::Color(sf::Uint32(0x1D4ED8));
const auto lime = sf::Color(sf::Uint32(0xA4E804));
const auto purple = sf::Color(sf::Uint32(0x9333EA));
const auto silver = sf::Color(sf::Uint32(0xC0C0C0));
const auto cyan = sf::Color(sf::Uint32(0x06B6D4));
const auto magenta = sf::Color(sf::Uint32(0xEC4899));
}  // namespace ui::color::light::property

namespace ui::color::light::ui {
// Light mode UI elements
const auto topBottomBar = sf::Color(sf::Uint32(0xFFFFFF));
const auto panelBackground = sf::Color(sf::Uint32(0xF9FAFB));
const auto panelBorder = sf::Color(sf::Uint32(0xE5E7EB));
}  // namespace ui::color::light::ui

namespace ui::color::light::text {
// Light mode text colors
const auto primary = sf::Color(sf::Uint32(0x111827));
const auto secondary = sf::Color(sf::Uint32(0x6B7280));
const auto inverse = sf::Color(sf::Uint32(0xFFFFFF));
}  // namespace ui::color::light::text

namespace ui::color::light::button {
// Light mode button colors
const auto primary = sf::Color(sf::Uint32(0x2563EB));
const auto primaryHover = sf::Color(sf::Uint32(0x1D4ED8));
const auto secondary = sf::Color(sf::Uint32(0xE5E7EB));
const auto secondaryHover = sf::Color(sf::Uint32(0xD1D5DB));
const auto disabled = sf::Color(sf::Uint32(0x9CA3AF));
}  // namespace ui::color::light::button

namespace ui::color::light::feedback {
// Light mode feedback colors
const auto success = sf::Color(sf::Uint32(0x22C55E));
const auto warning = sf::Color(sf::Uint32(0xF59E0B));
const auto error = sf::Color(sf::Uint32(0xEF4444));
const auto info = sf::Color(sf::Uint32(0x3B82F6));
}  // namespace ui::color::light::feedback

namespace ui::color::dark::surface {
// Dark mode core surfaces
const auto background = sf::Color(sf::Uint32(0x0B0F14));
const auto gameBoardBase = sf::Color(sf::Uint32(0x1B4D3E));
const auto boardBorder = sf::Color(sf::Uint32(0x1F2937));
const auto gridLines = sf::Color(sf::Uint32(0x374151));
}  // namespace ui::color::dark::surface

namespace ui::color::dark::property {
// Dark mode property tile colors (adjusted for contrast)
const auto brown = sf::Color(sf::Uint32(0xA47551));
const auto lightBlue = sf::Color(sf::Uint32(0x38BDF8));
const auto pink = sf::Color(sf::Uint32(0xF472B6));
const auto orange = sf::Color(sf::Uint32(0xFB923C));
const auto red = sf::Color(sf::Uint32(0xF87171));
const auto yellow = sf::Color(sf::Uint32(0xFDE047));
const auto green = sf::Color(sf::Uint32(0x4ADE80));
const auto darkBlue = sf::Color(sf::Uint32(0x60A5FA));
const auto lime = sf::Color(sf::Uint32(0xBEF264));
const auto purple = sf::Color(sf::Uint32(0xD946EF));
const auto silver = sf::Color(sf::Uint32(0xD1D5DB));
const auto cyan = sf::Color(sf::Uint32(0x22D3EE));
const auto magenta = sf::Color(sf::Uint32(0xF472B6));
}  // namespace ui::color::dark::property

namespace ui::color::dark::ui {
// Dark mode UI elements
const auto topBottomBar = sf::Color(sf::Uint32(0x111827));
const auto panelBackground = sf::Color(sf::Uint32(0x1F2937));
const auto panelBorder = sf::Color(sf::Uint32(0x374151));
}  // namespace ui::color::dark::ui

namespace ui::color::dark::text {
// Dark mode text colors
const auto primary = sf::Color(sf::Uint32(0xF9FAFB));
const auto secondary = sf::Color(sf::Uint32(0x9CA3AF));
const auto inverse = sf::Color(sf::Uint32(0x111827));
}  // namespace ui::color::dark::text

namespace ui::color::dark::button {
// Dark mode button colors
const auto primary = sf::Color(sf::Uint32(0x3B82F6));
const auto primaryHover = sf::Color(sf::Uint32(0x2563EB));
const auto secondary = sf::Color(sf::Uint32(0x374151));
const auto secondaryHover = sf::Color(sf::Uint32(0x4B5563));
const auto disabled = sf::Color(sf::Uint32(0x6B7280));
}  // namespace ui::color::dark::button

namespace ui::color::dark::feedback {
// Dark mode feedback colors
const auto success = sf::Color(sf::Uint32(0x4ADE80));
const auto warning = sf::Color(sf::Uint32(0xFBBF24));
const auto error = sf::Color(sf::Uint32(0xF87171));
const auto info = sf::Color(sf::Uint32(0x60A5FA));
}  // namespace ui::color::dark::feedback