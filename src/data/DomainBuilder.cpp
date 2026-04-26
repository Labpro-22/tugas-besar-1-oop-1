#include "data/DomainBuilder.hpp"

#include <map>

#include "core/GameException.hpp"

namespace data {

core::ColorGroup DomainBuilder::colorFromString(const std::string& s) {
    static const std::map<std::string, core::ColorGroup> table = {
        {"COKLAT",     core::ColorGroup::BROWN},
        {"BIRU_MUDA",  core::ColorGroup::LIGHT_BLUE},
        {"MERAH_MUDA", core::ColorGroup::PINK},
        {"ORANGE",     core::ColorGroup::ORANGE},
        {"MERAH",      core::ColorGroup::RED},
        {"KUNING",     core::ColorGroup::YELLOW},
        {"HIJAU",      core::ColorGroup::GREEN},
        {"BIRU_TUA",   core::ColorGroup::DARK_BLUE},
        {"ABU_ABU",    core::ColorGroup::GREY},
        {"UNGU",       core::ColorGroup::PURPLE},
        {"HIJAU_MUDA", core::ColorGroup::LIGHT_GREEN},
        {"GANDARIA",   core::ColorGroup::LAVENDER},
        {"PUTIH",      core::ColorGroup::WHITE},
        {"HITAM",      core::ColorGroup::BLACK},
        {"DEFAULT",    core::ColorGroup::DEFAULT},
    };
    auto it = table.find(s);
    if (it == table.end())
        throw InvalidConfigException("color",
            "valid color string (COKLAT/BIRU_MUDA/dll), got '" + s + "'");
    return it->second;
}

std::unique_ptr<core::Property> DomainBuilder::buildProperty(const PropertyConfig& cfg) {
    if (cfg.getType() == "STREET") {
        const auto& sc = static_cast<const StreetConfig&>(cfg);
        std::vector<int> rents(sc.rent, sc.rent + 6);
        core::ColorGroup cg = colorFromString(cfg.color);
        return std::make_unique<core::Street>(
            cfg.name, cfg.purchasePrice, cfg.mortgageValue,
            cg, rents, sc.houseUpgradeCost, sc.hotelUpgradeCost);
    }
    if (cfg.getType() == "RAILROAD") {
        // baseFare = 0 sementara; Game::initialize() set Railroad::rentTable
        // dari railroad.txt setelah semua tile dibangun.
        return std::make_unique<core::Railroad>(cfg.name, cfg.mortgageValue, 0);
    }
    if (cfg.getType() == "UTILITY") {
        return std::make_unique<core::Utility>(cfg.name, cfg.mortgageValue);
    }
    throw InvalidConfigException("property type",
        "STREET|RAILROAD|UTILITY, got '" + cfg.getType() + "'");
}

std::unique_ptr<core::Tile> DomainBuilder::buildActionTile(
    const ActionTileConfig& cfg, const TaxConfig& taxCfg)
{
    int pos = cfg.id - 1;  // config 1-indexed -> Tile 0-indexed

    if (cfg.tileType == "SPESIAL") {
        if (cfg.code == "GO")  return std::make_unique<core::GoTile>(pos, cfg.name);
        if (cfg.code == "PEN") return std::make_unique<core::JailTile>(pos, cfg.name);
        if (cfg.code == "BBP") return std::make_unique<core::FreeParkingTile>(pos, cfg.name);
        if (cfg.code == "PPJ") return std::make_unique<core::GoToJailTile>(pos, cfg.name);
        throw InvalidConfigException("aksi.txt SPESIAL code",
            "GO|PEN|BBP|PPJ, got '" + cfg.code + "'");
    }
    if (cfg.tileType == "KARTU") {
        // KSP = Kesempatan (Chance), DNU = Dana Umum (Community Chest)
        bool isChance = (cfg.code == "KSP");
        return std::make_unique<core::CardTile>(pos, cfg.name, isChance);
    }
    if (cfg.tileType == "PAJAK") {
        if (cfg.code == "PPH")
            return std::make_unique<core::TaxTile>(
                pos, cfg.name,
                taxCfg.incomeTaxFlat, taxCfg.incomeTaxPercent,
                core::TaxType::PPH);
        if (cfg.code == "PBM")
            return std::make_unique<core::TaxTile>(
                pos, cfg.name,
                taxCfg.luxuryTaxFlat, 0,
                core::TaxType::PBM);
        throw InvalidConfigException("aksi.txt PAJAK code",
            "PPH|PBM, got '" + cfg.code + "'");
    }
    if (cfg.tileType == "FESTIVAL") {
        return std::make_unique<core::FestivalTile>(pos, cfg.name);
    }
    throw InvalidConfigException("aksi.txt JENIS_PETAK",
        "SPESIAL|KARTU|PAJAK|FESTIVAL, got '" + cfg.tileType + "'");
}

std::vector<TileEntry> DomainBuilder::buildBoard(
    const std::vector<PropertyConfig*>& propCfgs,
    const std::vector<ActionTileConfig>& actionCfgs,
    const TaxConfig& taxCfg)
{
    // map key = 1-indexed position (dari config), untuk sorting otomatis
    std::map<int, TileEntry> tileMap;

    // Property tiles (STREET, RAILROAD, UTILITY)
    for (const auto* cfg : propCfgs) {
        if (!cfg) continue;

        if (tileMap.count(cfg->id))
            throw InvalidConfigException("property.txt",
                "duplicate position id=" + std::to_string(cfg->id));

        int pos = cfg->id - 1;  // 0-indexed untuk Tile
        auto prop = buildProperty(*cfg);

        TileEntry entry;
        entry.setCode(cfg->code);

        if (cfg->getType() == "RAILROAD") {
            entry.setTile(std::make_unique<core::RailroadTile>(pos, cfg->name, std::move(prop)));
        } else if (cfg->getType() == "UTILITY") {
            entry.setTile(std::make_unique<core::UtilityTile>(pos, cfg->name, std::move(prop)));
        } else {
            // STREET
            entry.setTile(std::make_unique<core::PropertyTile>(pos, cfg->name, std::move(prop)));
        }

        tileMap[cfg->id] = std::move(entry);
    }

    // Action tiles (GO, KARTU, PAJAK, FESTIVAL, SPESIAL lainnya)
    for (const auto& cfg : actionCfgs) {
        if (tileMap.count(cfg.id))
            throw InvalidConfigException("aksi.txt",
                "duplicate position id=" + std::to_string(cfg.id));

        TileEntry entry;
        entry.setCode(cfg.code);
        entry.setTile(buildActionTile(cfg, taxCfg));
        tileMap[cfg.id] = std::move(entry);
    }

    // Flatten map (sudah terurut by key = position) ke vector
    std::vector<TileEntry> result;
    result.reserve(tileMap.size());
    for (auto& [id, entry] : tileMap) {
        result.push_back(std::move(entry));
    }
    return result;
}

}  // namespace data