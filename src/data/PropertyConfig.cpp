#include "data/PropertyConfig.hpp"

#include <map>

#include "core/GameException.hpp"

namespace data {

core::ColorGroup PropertyConfig::colorFromString(const std::string& s) {
  static const std::map<std::string, core::ColorGroup> table = {
      {"COKLAT", core::ColorGroup::BROWN},
      {"BIRU_MUDA", core::ColorGroup::LIGHT_BLUE},
      {"MERAH_MUDA", core::ColorGroup::PINK},
      {"ORANGE", core::ColorGroup::ORANGE},
      {"MERAH", core::ColorGroup::RED},
      {"KUNING", core::ColorGroup::YELLOW},
      {"HIJAU", core::ColorGroup::GREEN},
      {"BIRU_TUA", core::ColorGroup::DARK_BLUE},
      {"ABU_ABU", core::ColorGroup::GREY},
      {"UNGU", core::ColorGroup::PURPLE},
      {"HIJAU_MUDA", core::ColorGroup::LIGHT_GREEN},
      {"GANDARIA", core::ColorGroup::LAVENDER},
      {"PUTIH", core::ColorGroup::WHITE},
      {"HITAM", core::ColorGroup::BLACK},
      {"DEFAULT", core::ColorGroup::DEFAULT},
  };

  auto it = table.find(s);
  if (it == table.end()) {
    throw InvalidConfigException(
        "color", "valid color string (COKLAT/BIRU_MUDA/dll), got '" + s + "'");
  }
  return it->second;
}

std::unique_ptr<core::Tile> StreetConfig::buildTile(int position) const {
  std::vector<int> rents(rent, rent + 6);
  return std::make_unique<core::StreetTile>(
      position, name,
      core::Street(name, purchasePrice, mortgageValue, colorFromString(color),
                   rents, houseUpgradeCost, hotelUpgradeCost));
}

std::unique_ptr<core::Tile> RailroadConfig::buildTile(int position) const {
  return std::make_unique<core::RailroadTile>(
      position, name, core::Railroad(name, mortgageValue, 0));
}

std::unique_ptr<core::Tile> UtilityConfig::buildTile(int position) const {
  return std::make_unique<core::UtilityTile>(
      position, name, core::Utility(name, mortgageValue));
}

}  // namespace data
