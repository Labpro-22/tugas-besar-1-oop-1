#pragma once
#include <vector>
#include <memory> 

namespace core { class Tile; }
namespace data { class ConfigReader; }

namespace logic {

class Board {
private:
    std::vector<std::unique_ptr<core::Tile>> tiles_;

public:
    Board() = default;
    ~Board(); 
    void loadFromConfig(data::ConfigReader& config);
    
    core::Tile* getTile(int index) const;
    int getTileCount() const;
    void addTile(std::unique_ptr<core::Tile> tile); 
};

} // namespace logic

