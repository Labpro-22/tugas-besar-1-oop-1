#include "logic/Board.hpp"
#include "core/Tiles.hpp"
// #include "data/ConfigReader.hpp"
namespace logic {

Board::~Board() = default;

void Board::addTile(std::unique_ptr<core::Tile> tile, const std::string& code) {
    if (!code.empty()) {
        codeIndex_[code] = static_cast<int>(tiles_.size());
    }
    tiles_.push_back(std::move(tile));
}

void Board::clear() {
    tiles_.clear();
    codeIndex_.clear();
}

core::Tile *Board::getTile(int index) const {
  if (index >= 0 && index < static_cast<int>(tiles_.size())) {
    return tiles_[index].get();
  }
  return nullptr; // exception for nonexisting Tile doesn't exist yet ... could use InvalidMove though but its redundant @ojen 
}

int Board::getTileCount() const { return static_cast<int>(tiles_.size()); }

core::Tile* Board::getTileByCode(const std::string& code) const {
    auto it = codeIndex_.find(code);
    if (it == codeIndex_.end()) return nullptr;
    return getTile(it->second);
}

} // namespace logic
