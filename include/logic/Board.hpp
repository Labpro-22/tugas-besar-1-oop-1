#pragma once
#include <vector>

namespace core { class Tile; }
namespace data { class ConfigReader; }

namespace logic {

class Board {
private:
    std::vector<core::Tile*> tiles_;

public:
    Board() = default;
    ~Board(); 
    void loadFromConfig(data::ConfigReader& config);
    
    core::Tile* getTile(int index) const;
    int getTileCount() const;
    void addTile(core::Tile* tile); 
};

} // namespace logic

