#pragma once
#include <string>
#include <vector>

namespace data {
class TileSpec {
 public:
  std::string code;  // mis: "GO", "GRT", "FES", "PPH"
};

class TaxConfig {
 public:
  int incomeTaxFlat = 0;
  int incomeTaxPercent = 0;
  int luxuryTaxFlat = 0;
};

class SpecialConfig {
 public:
  int goSalary = 0;
  int jailFine = 0;
};

class MiscConfig {
 public:
  int maxTurn = 0;
  int initialBalance = 0;
};

class BoardConfig {
 public:
  std::vector<TileSpec> tileOrder;
};

class ActionTileConfig {
 public:
  int id = 0;
  std::string code;
  std::string name;
  std::string tileType;
  std::string color;
};
}  // namespace data