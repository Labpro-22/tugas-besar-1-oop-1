#pragma once
#include <string>

namespace data {
// dibaca dari property.txt
class PropertyConfig {
 public:
  int id = 0;
  std::string code;   // kode huruf unik, misal: "GRT"
  std::string name;   // nama lengkap, misal: "GARUT"
  std::string type;   // "STREET" | "RAILROAD" | "UTILITY"
  std::string color; 
  int purchasePrice = 0;
  int mortgageValue = 0;

  virtual ~PropertyConfig() = default;
  virtual std::string getType() const = 0;
};

// kolom property.txt setelah mortgageValue:
// UPG_RUMAH  UPG_HT  RENT_L0 … RENT_L5
class StreetConfig : public PropertyConfig {
 public:
  int houseUpgradeCost = 0;
  int hotelUpgradeCost = 0;
  int rent[6] = {};  // index 0 = unimproved … 5 = hotel

  std::string getType() const override { return "STREET"; }
};

// dilihat dari railroad.txt
class RailroadConfig : public PropertyConfig {
 public:
  std::string getType() const override { return "RAILROAD"; }
};

// dilihat dari utility.txt
class UtilityConfig : public PropertyConfig {
 public:
  std::string getType() const override { return "UTILITY"; }
};
}  // namespace data
