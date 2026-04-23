#pragma once
#include <string>
#include <vector>

#include "data/LogEntry.hpp"

namespace logic {
// Declare stub sementara, full class ada di include/core/
class Player;
class Property;

class TransactionLogger {
 public:
  void log(int turnNumber, const std::string& actionType, const Player& player,
           int amount = 0);

  void log(int turnNumber, const std::string& actionType, const Player& player,
           const Property& property, int amount = 0);

  // Overload incase ada deskripsi bebas
  void log(int turnNumber, const std::string& actionType, const Player& player,
           const std::string& description);

  const std::vector<data::LogEntry>& getLog() const;

  // Ambil n entries terakhir untuk CETAK_LOG <n> (n=0 artinya semua)
  std::vector<data::LogEntry> getRecentLog(int n = 0) const;

  void clear();

 private:
  std::vector<data::LogEntry> entries_;
};
}  // namespace logic
