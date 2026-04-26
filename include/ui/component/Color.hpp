#pragma once

#include <SFML/Config.hpp>
#include <SFML/Graphics/Color.hpp>

namespace ui::palette {
const auto white = sf::Color(sf::Uint32(0xF2F2F2FFU));
const auto white40 = sf::Color(sf::Uint32(0xF2F2F266U));
const auto lightGrey = sf::Color(sf::Uint32(0xCCCCCCFFU));
const auto darkGrey = sf::Color(sf::Uint32(0x7F7F7FFFU));
const auto backgroundGrey = sf::Color(sf::Uint32(0x595959FFU));
const auto black = sf::Color(sf::Uint32(0x050505FFU));
const auto black20 = sf::Color(sf::Uint32(0x05050533U));
}  // namespace ui::palette

namespace ui::accent {
const auto pink = sf::Color(sf::Uint32(0xD93A96FFU));
const auto red = sf::Color(sf::Uint32(0xF11C26FFU));
const auto darkRed = sf::Color(sf::Uint32(0xB30008FFU));
const auto orange = sf::Color(sf::Uint32(0xFF8B00FFU));
const auto orange50 = sf::Color(sf::Uint32(0xFF8B0080U));
const auto darkOrange = sf::Color(sf::Uint32(0XDF7A00FFU));
const auto yellow = sf::Color(sf::Uint32(0xFEF200FFU));
const auto mint = sf::Color(sf::Uint32(0xD9E8D6FFU));
const auto lightGreen = sf::Color(sf::Uint32(0x91CB3EFFU));
const auto green = sf::Color(sf::Uint32(0x1FB25AFFU));
const auto darkGreen = sf::Color(sf::Uint32(0x00762FFFU));
const auto lightBlue = sf::Color(sf::Uint32(0xAAE0FAFFU));
const auto blue50 = sf::Color(sf::Uint32(0x0072BB80U));
const auto darkBlue = sf::Color(sf::Uint32(0x0072BBFFU));
const auto purple = sf::Color(sf::Uint32(0x4B36D3FFU));
const auto lavender = sf::Color(sf::Uint32(0XD295BFFFU));
const auto brown = sf::Color(sf::Uint32(0x955235FFU));
const auto darkBrown = sf::Color(sf::Uint32(0x391F14FFU));
}  // namespace ui::accent

namespace ui::component {
const auto background = palette::white;
const auto sideBar = accent::lightBlue;
const auto card = palette::darkGrey;
const auto cardHover = palette::lightGrey;
const auto panel = accent::darkBrown;

namespace button {
const auto base = palette::black;
const auto hover = palette::black20;
const auto confirm = accent::green;
const auto cancel = accent::red;
const auto disabled = palette::lightGrey;

}  // namespace button
}  // namespace ui::component

namespace ui::board {
const auto base = accent::mint;
const auto baseHover = sf::Color(sf::Uint32(0xD9E8D680U));
const auto border = palette::black;

namespace property {
const auto brown = accent::brown;
const auto lightBlue = accent::lightBlue;
const auto pink = accent::pink;
const auto orange = accent::orange;
const auto red = accent::red;
const auto yellow = accent::yellow;
const auto green = accent::green;
const auto darkBlue = accent::darkBlue;
const auto grey = palette::darkGrey;
const auto purple = accent::purple;
const auto lightGreen = accent::lightGreen;
const auto lavender = accent::lavender;
const auto white = palette::white;
const auto black = palette::black;
const auto defaultColor = accent::mint;
}  // namespace property
}  // namespace ui::board