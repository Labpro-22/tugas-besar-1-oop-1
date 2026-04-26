#pragma once
#include <map>
#include <string>
#include <vector>

#include "data/ConfigStructs.hpp"
#include "data/PropertyConfig.hpp"

namespace data {
class ConfigReader {
 public:
  /**
   * @param baseConfigPath Path ke folder config (misal: "config/")
   * @param boardSize Ukuran papan (20, 24, ..., 60). Default 40.
   */
  explicit ConfigReader(const std::string& baseConfigPath, int boardSize = 40);

  std::vector<PropertyConfig*> readProperties();
  std::map<int, int> readRailroadRents();
  std::map<int, int> readUtilityMultipliers();
  TaxConfig readTax();
  SpecialConfig readSpecial();
  MiscConfig readMisc();
  std::vector<ActionTileConfig> readActionTiles();
  BoardConfig readBoardConfig();

 private:
  std::string basePath_;
  int boardSize_;

  /**
   * @brief Resolve path file config.
   * File board-specific -> basePath_/board_{boardSize_}/filename
   * File shared         -> basePath_/filename
   */
  std::string filePath(const std::string& filename) const;
};

}  // namespace data