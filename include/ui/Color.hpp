#pragma once

#include <SFML/Config.hpp>
#include <SFML/Graphics/Color.hpp>

namespace ui::palette {
const auto white = sf::Color(sf::Uint32(0xF2F2F2FFU));
const auto lightGrey = sf::Color(sf::Uint32(0xCCCCCCFFU));
const auto darkGrey = sf::Color(sf::Uint32(0x7F7F7FFFU));
const auto backgroundGrey = sf::Color(sf::Uint32(0x595959FFU));
}  // namespace ui::palette

namespace ui::accent {
const auto pink = sf::Color(sf::Uint32(0xE55D8DFFU));
const auto red = sf::Color(sf::Uint32(0xD81159FFU));
const auto orange = sf::Color(sf::Uint32(0xEC674EFFU));
const auto yellow = sf::Color(sf::Uint32(0xFFBC42FFU));
const auto lime = sf::Color(sf::Uint32(0x6CC551FFU));
const auto green = sf::Color(sf::Uint32(0x447604FFU));
const auto cyan = sf::Color(sf::Uint32(0x9FFCDFFFU));
const auto lightBlue = sf::Color(sf::Uint32(0x0496FFFFU));
const auto darkBlue = sf::Color(sf::Uint32(0x006BA6FFU));
const auto lavender = sf::Color(sf::Uint32(0x70587CFFU));
const auto magenta = sf::Color(sf::Uint32(0xE800E8FFU));
const auto purple = sf::Color(sf::Uint32(0x2E1760FFU));
const auto brown = sf::Color(sf::Uint32(0x8B5A2BFFU));
const auto black = sf::Color(sf::Uint32(0x040E18FFU));
}  // namespace ui::accent

namespace ui::component {
const auto background = palette::white;
const auto sideBar = palette::backgroundGrey;
const auto card = palette::darkGrey;
const auto cardHover = palette::lightGrey;

namespace button {
const auto base = palette::darkGrey;
const auto hover = palette::lightGrey;
const auto yes = accent::lime;
const auto no = accent::red;
const auto disabled = palette::lightGrey;
const auto green = accent::green;
const auto redAction = accent::red;
const auto purple = accent::lavender;
const auto blue = accent::lightBlue;
const auto orange = accent::orange;
const auto lightBlue = accent::lightBlue;
}  // namespace button

const auto board = sf::Color(sf::Uint32(0xA1CCA5FFU));
const auto border = accent::black;
}  // namespace ui::component

namespace ui::property {
const auto brown = accent::brown;
const auto lightBlue = accent::lightBlue;
const auto pink = accent::pink;
const auto orange = accent::orange;
const auto red = accent::red;
const auto yellow = accent::yellow;
const auto green = accent::green;
const auto darkBlue = accent::darkBlue;
const auto lime = accent::lime;
const auto purple = accent::purple;
const auto silver = palette::lightGrey;
const auto cyan = accent::cyan;
const auto magenta = accent::magenta;
}  // namespace ui::property
