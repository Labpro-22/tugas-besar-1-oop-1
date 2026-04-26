#include "data/ConfigStructs.hpp"

#include "core/GameException.hpp"
#include "core/Tiles.hpp"

namespace data {

std::unique_ptr<core::Tile> ActionTileConfig::buildTile(
    const TaxConfig& taxCfg) const {
  const int pos = id - 1;

  if (tileType == ActionTileType::SPECIAL) {
    if (code == "GO") return std::make_unique<core::GoTile>(pos, name);
    if (code == "PEN") return std::make_unique<core::JailTile>(pos, name);
    if (code == "BBP") {
      return std::make_unique<core::FreeParkingTile>(pos, name);
    }
    if (code == "PPJ") {
      return std::make_unique<core::GoToJailTile>(pos, name);
    }
    throw InvalidConfigException("aksi.txt SPESIAL code",
                                 "GO|PEN|BBP|PPJ, got '" + code + "'");
  }

  if (tileType == ActionTileType::CARD) {
    const bool isChance = (code == "KSP");
    return std::make_unique<core::CardTile>(pos, name, isChance);
  }

  if (tileType == ActionTileType::TAX) {
    if (code == "PPH") {
      return std::make_unique<core::TaxTile>(pos, name, taxCfg.incomeTaxFlat,
                                             taxCfg.incomeTaxPercent,
                                             core::TaxType::PPH);
    }
    if (code == "PBM") {
      return std::make_unique<core::TaxTile>(pos, name, taxCfg.luxuryTaxFlat, 0,
                                             core::TaxType::PBM);
    }
    throw InvalidConfigException("aksi.txt PAJAK code",
                                 "PPH|PBM, got '" + code + "'");
  }

  if (tileType == ActionTileType::FESTIVAL) {
    return std::make_unique<core::FestivalTile>(pos, name);
  }

  throw InvalidConfigException("aksi.txt JENIS_PETAK",
                               "SPESIAL|KARTU|PAJAK|FESTIVAL");
}

}  // namespace data
