#pragma once

#include <SFML/Graphics/Color.hpp>
#include <string>

namespace core {
class Property;
enum class ColorGroup : int;
}

namespace ui::dialog_detail {

float buttonWidth();
sf::Color groupColor(core::ColorGroup group);
sf::Color propertyBandColor(const core::Property& property);
std::string formatMoney(int amount);

}  // namespace ui::dialog_detail
