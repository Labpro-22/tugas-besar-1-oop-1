#pragma once
#include <map>
#include <string>
#include <vector>

#include "data/ConfigStructs.hpp"
#include "data/PropertyConfig.hpp"

namespace data {
class ConfigReader {
 public:
  explicit ConfigReader(const std::string& basePath);

  // mengembalikan heap-allocated configs
  std::vector<PropertyConfig*> readProperties();

  // railroad.txt: count -> rent
  std::map<int, int> readRailroadRents();

  // utility.txt: count -> multiplier
  std::map<int, int> readUtilityMultipliers();

  TaxConfig readTax();
  SpecialConfig readSpecial();
  MiscConfig readMisc();

  // aksi.txt: daftar action tile beserta atributnya
  std::vector<ActionTileConfig> readActionTiles();

  // untuk Dynamic Board bonus: board.txt -> ordered tile list (20-60 tiles)
  BoardConfig readBoardConfig();

 private:
  std::string basePath_;

  std::string filePath(const std::string& filename) const;
};
}  // namespace data