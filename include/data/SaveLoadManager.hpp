#pragma once
#include <string>
#include "data/GameStateDTO.hpp"

// Setelah load(), PropertyDTO::id == -1.
// Game harus resolve index via PropertyDTO::code menggunakan Board::getTileIndexByCode() atau sejenisnya.
class SaveLoadManager {
public:
    void save(const GameStateDTO& state, const std::string& filename) const;

    GameStateDTO load(const std::string& filename) const;
};
