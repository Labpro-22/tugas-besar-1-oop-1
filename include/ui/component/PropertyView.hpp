#pragma once

#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <cctype>
#include <string>

#include "core/Property.hpp"
#include "core/Tiles.hpp"
#include "ui/component/Color.hpp"

namespace ui {

struct PropertyView {
  sf::Color groupColor = board::property::defaultColor;
  std::string code;
  int houseCount = 0;
  bool isRailroad = false;
  bool isUtility = false;
  std::string utilityIconPath;
};

namespace property_view_detail {

inline sf::Color mapColor(core::ColorGroup group) {
  using CG = core::ColorGroup;
  switch (group) {
    case CG::BROWN:
      return board::property::brown;
    case CG::LIGHT_BLUE:
      return board::property::lightBlue;
    case CG::PINK:
      return board::property::pink;
    case CG::ORANGE:
      return board::property::orange;
    case CG::RED:
      return board::property::red;
    case CG::YELLOW:
      return board::property::yellow;
    case CG::GREEN:
      return board::property::green;
    case CG::DARK_BLUE:
      return board::property::darkBlue;
    case CG::GREY:
      return board::property::grey;
    case CG::PURPLE:
      return board::property::purple;
    case CG::LIGHT_GREEN:
      return board::property::lightGreen;
    case CG::LAVENDER:
      return board::property::lavender;
    case CG::WHITE:
      return board::property::white;
    case CG::BLACK:
      return board::property::black;
    default:
      return board::property::defaultColor;
  }
}

inline std::string makeCode(const std::string& name) {
  std::string code;
  code.reserve(3);

  bool takeNext = true;
  for (char ch : name) {
    if (std::isspace(static_cast<unsigned char>(ch))) {
      takeNext = true;
      continue;
    }
    if (takeNext && std::isalpha(static_cast<unsigned char>(ch))) {
      code.push_back(static_cast<char>(
          std::toupper(static_cast<unsigned char>(ch))));
      takeNext = false;
      if (code.size() == 3) return code;
    }
  }

  if (code.empty()) {
    for (char ch : name) {
      if (std::isalnum(static_cast<unsigned char>(ch))) {
        code.push_back(static_cast<char>(
            std::toupper(static_cast<unsigned char>(ch))));
        if (code.size() == 3) break;
      }
    }
  }

  return code.empty() ? std::string("N/A") : code;
}

inline std::string utilityIconFor(const std::string& name) {
  std::string lowered = name;
  std::transform(lowered.begin(), lowered.end(), lowered.begin(),
                 [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

  if (lowered.find("air") != std::string::npos ||
      lowered.find("water") != std::string::npos) {
    return "assets/icons/water.png";
  }
  if (lowered.find("pln") != std::string::npos) {
    return "assets/icons/electricity-pln.png";
  }
  return "assets/icons/electricity.png";
}

}  // namespace property_view_detail

inline PropertyView buildPropertyView(const core::Property& property) {
  PropertyView view;
  view.code = property_view_detail::makeCode(property.getName());

  switch (property.getType()) {
    case core::PropertyTileType::STREET: {
      const auto& street = static_cast<const core::Street&>(property);
      view.groupColor = property_view_detail::mapColor(street.getColorGroup());
      view.houseCount = street.getHouseCount() + street.getHotelCount() * 5;
      break;
    }
    case core::PropertyTileType::RAILROAD:
      view.groupColor = board::property::black;
      view.isRailroad = true;
      break;
    case core::PropertyTileType::UTILITY:
      view.groupColor = board::property::grey;
      view.isUtility = true;
      view.utilityIconPath = property_view_detail::utilityIconFor(property.getName());
      break;
    default:
      break;
  }

  return view;
}

}  // namespace ui
