#pragma once
#include <memory>
#include <string>

#include "core/Tiles.hpp"

namespace data {
// dibaca dari property.txt
class PropertyConfig {
 public:
  int id = 0;
  std::string code;  // kode huruf unik, misal: "GRT"
  std::string name;  // nama lengkap, misal: "GARUT"
  std::string type;  // "STREET" | "RAILROAD" | "UTILITY"
  std::string color;
  int purchasePrice = 0;
  int mortgageValue = 0;

  virtual ~PropertyConfig() = default;
  virtual std::string getType() const = 0;
  virtual std::unique_ptr<core::Tile> buildTile(int position) const = 0;

 protected:
  static core::ColorGroup colorFromString(const std::string& s);
};

// kolom property.txt setelah mortgageValue:
// UPG_RUMAH  UPG_HT  RENT_L0 ... RENT_L5
class StreetConfig : public PropertyConfig {
 public:
  int houseUpgradeCost = 0;
  int hotelUpgradeCost = 0;
  int rent[6] = {};  // index 0 = unimproved ... 5 = hotel

  std::string getType() const override { return "STREET"; }
  std::unique_ptr<core::Tile> buildTile(int position) const override;
};

// dilihat dari railroad.txt
class RailroadConfig : public PropertyConfig {
 public:
  std::string getType() const override { return "RAILROAD"; }
  std::unique_ptr<core::Tile> buildTile(int position) const override;
};

// dilihat dari utility.txt
class UtilityConfig : public PropertyConfig {
 public:
  std::string getType() const override { return "UTILITY"; }
  std::unique_ptr<core::Tile> buildTile(int position) const override;
};
}  // namespace data
