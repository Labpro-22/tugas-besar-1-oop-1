#include "logic/Board.hpp"
#include "core/Tiles.hpp"
// #include "data/ConfigReader.hpp"
namespace logic {

Board::~Board() {
  // here we clear the entire tiles because board owns tiles in its class
  for (core::Tile *tile : tiles_) {
    delete tile;
  }
  tiles_.clear();
}

// logging logging 
// void Board::loadFromConfig(data::ConfigReader& config) {
//     // data::BoardConfig bConfig = config.readBoardConfig();
//
//     // for(auto* t : tiles_) delete t;
//     // tiles_.clear();
//
// }

core::Tile *Board::getTile(int index) const {
  if (index >= 0 && index < static_cast<int>(tiles_.size())) {
    return tiles_[index];
  }
  return nullptr; // exception for nonexisting Tile doesn't exist yet ... could use InvalidMove though but its redundant @ojen 
}

int Board::getTileCount() const { return static_cast<int>(tiles_.size()); }

void Board::addTile(core::Tile* tile){ 
	tiles_.push_back(tile);
} 

} // namespace logic
