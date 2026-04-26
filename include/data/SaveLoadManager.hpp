#pragma once
#include <string>

#include "data/GameStateDTO.hpp"

namespace data {
// Setelah load(), PropertyDTO::id == -1.
// Game harus resolve index via PropertyDTO::code menggunakan
// Board::getTileIndexByCode() atau sejenisnya.
class SaveLoadManager {
 public:
  SaveLoadManager(const SaveLoadManager&) = delete;
  SaveLoadManager& operator=(const SaveLoadManager&) = delete;
  SaveLoadManager(SaveLoadManager&&) = delete;
  SaveLoadManager& operator=(SaveLoadManager&&) = delete;

  static SaveLoadManager& get() {
    static SaveLoadManager instance;
    return instance;
  }

  void save(const GameStateDTO& state, const std::string& filename) const;

  GameStateDTO load(const std::string& filename) const;

 private:
  SaveLoadManager() = default;
};
}  // namespace data