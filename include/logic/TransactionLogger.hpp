#pragma once
#include <string>
#include <vector>

#include "core/Property.hpp"
#include "core/player/Player.hpp"
#include "data/LogEntry.hpp"

namespace logic {

class TransactionLogger {
 public:
  void log(int turnNumber, const std::string& actionType,
           const core::Player& player, int amount = 0);

  void log(int turnNumber, const std::string& actionType,
           const core::Player& player, const core::Property& property,
           int amount = 0);

  void log(int turnNumber, const std::string& actionType,
           const core::Player& player, const std::string& description);

  const std::vector<data::LogEntry>& getLog() const;

  std::vector<data::LogEntry> getRecentLog(int n = 0) const;

  void clear();

  /**
   * @brief Restore seluruh log dari entries yang dimuat dari save file.
   * Menggantikan semua entries yang ada.
   * Dipanggil oleh Game::restoreLog() -> GameLoader::restoreGameMeta().
   */
  void restore(const std::vector<data::LogEntry>& entries);

 private:
  std::vector<data::LogEntry> entries_;
};

}  // namespace logic