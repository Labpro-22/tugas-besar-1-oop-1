#pragma once
#include <string>
#include <vector>
#include <map>
#include "data/PropertyConfig.hpp"
#include "data/ConfigStructs.hpp"

class ConfigReader {
public:
    explicit ConfigReader(const std::string& basePath);

    // mengembalikan heap-allocated configs
    std::vector<PropertyConfig*> readProperties();

    // railroad.txt: count -> rent
    std::map<int, int> readRailroadRents();

    // utility.txt: count -> multiplier
    std::map<int, int> readUtilityMultipliers();

    TaxConfig     readTax();
    SpecialConfig readSpecial();
    MiscConfig    readMisc();

    // untuk Dynamic Board bonus: board.txt -> ordered tile list (20–60 tiles)
    BoardConfig   readBoardConfig();

private:
    std::string basePath_;

    std::string filePath(const std::string& filename) const;
};
