#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "core/Property.hpp"
#include "core/Tiles.hpp"
#include "data/ConfigStructs.hpp"
#include "data/PropertyConfig.hpp"

namespace data {

// Pasangan tile + code-nya untuk registrasi ke Board
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

/**
 * @brief Converts raw config objects (from ConfigReader) into core domain objects.
 *
 * Tanggung jawab:
 *   - String warna config ("COKLAT") -> enum ColorGroup
 *   - PropertyConfig* -> concrete Property subclass (Street/Railroad/Utility)
 *   - ActionTileConfig -> concrete Tile subclass (GoTile/TaxTile/dll)
 *   - Menggabungkan property.txt + aksi.txt menjadi satu vector tile terurut
 *
 * Stateless. semua method static.
 */
class DomainBuilder {
public:
    /**
     * @brief Konversi string warna dari config ke enum ColorGroup.
     * @throws InvalidConfigException jika string tidak dikenali.
     */
    static core::ColorGroup colorFromString(const std::string& s);

    /**
     * @brief Buat concrete Property dari PropertyConfig.
     * Untuk Railroad: baseFare di-set 0 (placeholder).
     * Game::initialize() akan memanggil Railroad::setRentTable() setelah ini.
     * @throws InvalidConfigException jika type tidak valid.
     */
    static std::unique_ptr<core::Property> buildProperty(const PropertyConfig& cfg);

    /**
     * @brief Gabungkan property.txt dan aksi.txt menjadi vector TileEntry terurut posisi.
     *
     * Algoritma:
     *   1. Masukkan semua PropertyConfig ke map<position, TileEntry>
     *   2. Masukkan semua ActionTileConfig ke map yang sama
     *   3. Flatten map (sudah terurut ascending by key) ke vector
     *
     * Position dalam config adalah 1-indexed; Tile::position_ adalah 0-indexed.
     *
     * @throws InvalidConfigException jika ada posisi duplikat atau tipe tidak dikenali.
     */
    static std::vector<TileEntry> buildBoard(
        const std::vector<PropertyConfig*>& propCfgs,
        const std::vector<ActionTileConfig>& actionCfgs,
        const TaxConfig& taxCfg);

private:
    static std::unique_ptr<core::Tile> buildActionTile(
        const ActionTileConfig& cfg, const TaxConfig& taxCfg);
};

}  // namespace data