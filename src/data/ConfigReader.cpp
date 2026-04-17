#include "data/ConfigReader.hpp"

#include <fstream>
#include <sstream>

#include "core/GameException.hpp"

namespace data {
ConfigReader::ConfigReader(const std::string& basePath) : basePath_(basePath) {}

std::string ConfigReader::filePath(const std::string& filename) const {
  return basePath_ + "/" + filename;
}

static std::string trimCR(std::string s) {
  while (!s.empty() && (s.back() == '\r' || s.back() == ' ')) s.pop_back();
  return s;
}

// property.txt
std::vector<PropertyConfig*> ConfigReader::readProperties() {
  std::ifstream file(filePath("property.txt"));
  if (!file.is_open())
    throw InvalidConfigException{
        "property.txt", "readable file at " + filePath("property.txt")};

  std::vector<PropertyConfig*> configs;
  std::string line;
  std::getline(file, line);  // skip header

  while (std::getline(file, line)) {
    line = trimCR(line);
    if (line.empty()) continue;

    std::istringstream ss(line);
    int id;
    std::string code, name, type, color;
    ss >> id >> code >> name >> type >> color;

    if (type == "STREET") {
      auto* cfg = new StreetConfig();
      cfg->id = id;
      cfg->code = code;
      cfg->name = name;
      cfg->type = type;
      cfg->color = color;
      ss >> cfg->purchasePrice >> cfg->mortgageValue >> cfg->houseUpgradeCost >>
          cfg->hotelUpgradeCost;
      for (int i = 0; i < 6; ++i) ss >> cfg->rent[i];

      if (ss.fail()) {
        for (auto* c : configs) delete c;
        throw InvalidConfigException{"property.txt",
                                     "complete STREET row for " + code};
      }
      configs.push_back(cfg);

    } else if (type == "RAILROAD") {
      auto* cfg = new RailroadConfig();
      cfg->id = id;
      cfg->code = code;
      cfg->name = name;
      cfg->type = type;
      cfg->color = color;
      cfg->purchasePrice = 0;
      int hargaLahan;
      ss >> hargaLahan >> cfg->mortgageValue;

      if (ss.fail()) {
        for (auto* c : configs) delete c;
        throw InvalidConfigException{"property.txt",
                                     "complete RAILROAD row for " + code};
      }
      configs.push_back(cfg);

    } else if (type == "UTILITY") {
      auto* cfg = new UtilityConfig();
      cfg->id = id;
      cfg->code = code;
      cfg->name = name;
      cfg->type = type;
      cfg->color = color;
      cfg->purchasePrice = 0;
      int hargaLahan;
      ss >> hargaLahan >> cfg->mortgageValue;

      if (ss.fail()) {
        for (auto* c : configs) delete c;
        throw InvalidConfigException{"property.txt",
                                     "complete UTILITY row for " + code};
      }
      configs.push_back(cfg);

    } else {
      for (auto* c : configs) delete c;
      throw InvalidConfigException{
          "property.txt",
          "valid JENIS (STREET/RAILROAD/UTILITY), got '" + type + "'"};
    }
  }

  return configs;
}

// railroad.txt
std::map<int, int> ConfigReader::readRailroadRents() {
  std::ifstream file(filePath("railroad.txt"));
  if (!file.is_open())
    throw InvalidConfigException{"railroad.txt", "readable file"};

  std::map<int, int> rents;
  std::string line;
  std::getline(file, line);  // skip header

  while (std::getline(file, line)) {
    line = trimCR(line);
    if (line.empty()) continue;
    std::istringstream ss(line);
    int count, rent;
    ss >> count >> rent;
    if (ss.fail())
      throw InvalidConfigException{"railroad.txt", "two integers per row"};
    rents[count] = rent;
  }
  return rents;
}

// utility.txt
std::map<int, int> ConfigReader::readUtilityMultipliers() {
  std::ifstream file(filePath("utility.txt"));
  if (!file.is_open())
    throw InvalidConfigException{"utility.txt", "readable file"};

  std::map<int, int> multipliers;
  std::string line;
  std::getline(file, line);  // skip header

  while (std::getline(file, line)) {
    line = trimCR(line);
    if (line.empty()) continue;
    std::istringstream ss(line);
    int count, mult;
    ss >> count >> mult;
    if (ss.fail())
      throw InvalidConfigException{"utility.txt", "two integers per row"};
    multipliers[count] = mult;
  }
  return multipliers;
}

// tax.txt
TaxConfig ConfigReader::readTax() {
  std::ifstream file(filePath("tax.txt"));
  if (!file.is_open()) throw InvalidConfigException{"tax.txt", "readable file"};

  TaxConfig cfg;
  std::string line;
  std::getline(file, line);  // skip header

  if (!std::getline(file, line))
    throw InvalidConfigException{"tax.txt", "data row after header"};

  std::istringstream ss(trimCR(line));
  ss >> cfg.incomeTaxFlat >> cfg.incomeTaxPercent >> cfg.luxuryTaxFlat;
  if (ss.fail())
    throw InvalidConfigException{
        "tax.txt", "three integers: PPH_FLAT PPH_PERSENTASE PBM_FLAT"};
  return cfg;
}

// special.txt
SpecialConfig ConfigReader::readSpecial() {
  std::ifstream file(filePath("special.txt"));
  if (!file.is_open())
    throw InvalidConfigException{"special.txt", "readable file"};

  SpecialConfig cfg;
  std::string line;
  std::getline(file, line);  // skip header

  if (!std::getline(file, line))
    throw InvalidConfigException{"special.txt", "data row after header"};

  std::istringstream ss(trimCR(line));
  ss >> cfg.goSalary >> cfg.jailFine;
  if (ss.fail())
    throw InvalidConfigException{"special.txt",
                                 "two integers: GO_SALARY JAIL_FINE"};
  return cfg;
}

// misc.txt
MiscConfig ConfigReader::readMisc() {
  std::ifstream file(filePath("misc.txt"));
  if (!file.is_open())
    throw InvalidConfigException{"misc.txt", "readable file"};

  MiscConfig cfg;
  std::string line;
  std::getline(file, line);  // skip header

  if (!std::getline(file, line))
    throw InvalidConfigException{"misc.txt", "data row after header"};

  std::istringstream ss(trimCR(line));
  ss >> cfg.maxTurn >> cfg.initialBalance;
  if (ss.fail())
    throw InvalidConfigException{"misc.txt",
                                 "two integers: MAX_TURN SALDO_AWAL"};
  return cfg;
}

// board.txt (bonus Dynamic Board)
// Line 1 : JUMLAH_PETAK
// Line 2+: satu tile code pada tiap line
BoardConfig ConfigReader::readBoardConfig() {
  std::ifstream file(filePath("board.txt"));
  if (!file.is_open())
    throw InvalidConfigException{"board.txt", "readable file"};

  std::string line;
  if (!std::getline(file, line))
    throw InvalidConfigException{"board.txt", "tile count on first line"};

  int tileCount = 0;
  try {
    tileCount = std::stoi(trimCR(line));
  } catch (...) {
    throw InvalidConfigException{"board.txt",
                                 "integer tile count on first line"};
  }

  if (tileCount < 20 || tileCount > 60)
    throw InvalidConfigException{"board.txt", "tile count between 20 and 60"};

  BoardConfig cfg;
  int goCount = 0;
  int jailCount = 0;

  while (std::getline(file, line)) {
    std::string code = trimCR(line);
    if (code.empty()) continue;

    if (code == "GO") ++goCount;
    if (code == "PEN") ++jailCount;

    cfg.tileOrder.push_back(TileSpec{code});
  }

  // validasi agar jumlah GO dan PEN = 1
  if (goCount != 1)
    throw InvalidConfigException{"board.txt", "exactly 1 GO tile"};
  if (jailCount != 1)
    throw InvalidConfigException{"board.txt", "exactly 1 PEN (Jail) tile"};
  if (static_cast<int>(cfg.tileOrder.size()) != tileCount)
    throw InvalidConfigException{
        "board.txt",
        std::to_string(tileCount) + " tile entries matching declared count"};

  return cfg;
}
}  // namespace data