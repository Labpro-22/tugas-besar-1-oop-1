#pragma once
#include <memory>
#include <string>
#include <vector>

#include "core/Tiles.hpp"

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

enum class ActionTileType { SPECIAL, CARD, TAX, FESTIVAL };

class ActionTileConfig {
 public:
  int id = 0;
  std::string code;
  std::string name;
  ActionTileType tileType = ActionTileType::SPECIAL;
  std::string color;

  std::unique_ptr<core::Tile> buildTile(const TaxConfig& taxCfg) const;
};

// Pair tile + code untuk registrasi Board::codeIndex_.
class TileEntry {
 private:
  std::unique_ptr<core::Tile> tile_;
  std::string code_;

 public:
  TileEntry() = default;
  TileEntry(std::unique_ptr<core::Tile> tile, std::string code)
      : tile_(std::move(tile)), code_(std::move(code)) {}

  TileEntry(TileEntry&&) = default;
  TileEntry& operator=(TileEntry&&) = default;

  core::Tile* getTile() const { return tile_.get(); }
  std::unique_ptr<core::Tile> releaseTile() { return std::move(tile_); }
  void setTile(std::unique_ptr<core::Tile> tile) { tile_ = std::move(tile); }

  const std::string& getCode() const { return code_; }
  void setCode(const std::string& code) { code_ = code; }
};
}  // namespace data