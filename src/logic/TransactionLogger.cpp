#include "logic/TransactionLogger.hpp"
// komenin dulu biar bisa jalan
// #include "core/Player.hpp"
// #include "core/Property.hpp"

namespace logic {  // stub class

// TODO: HAPUS KETIKA KELAS UTAMA SUDAH JADI
class Player {
 public:
  std::string getName() const { return ""; }
};

// TODO: HAPUS KETIKA KELAS UTAMA SUDAH JADI
class Property {
 public:
  std::string getName() const { return ""; }
  std::string getCode() const { return ""; }
};

// ── Overload 1: player action with amount ──────────────────────────────────
void TransactionLogger::log(int turnNumber, const std::string& actionType,
                            const Player& player, int amount) {
  data::LogEntry entry;
  entry.turnNumber = turnNumber;
  entry.playerName = player.getName();
  entry.actionType = actionType;
  entry.description = (amount != 0) ? "M" + std::to_string(amount) : "";
  entries_.push_back(entry);
}

// ── Overload 2: player action involving a property ─────────────────────────
void TransactionLogger::log(int turnNumber, const std::string& actionType,
                            const Player& player, const Property& property,
                            int amount) {
  data::LogEntry entry;
  entry.turnNumber = turnNumber;
  entry.playerName = player.getName();
  entry.actionType = actionType;

  // e.g. "Jakarta (JKT) M600"
  entry.description = property.getName() + " (" + property.getCode() + ")";
  if (amount != 0) entry.description += " M" + std::to_string(amount);

  entries_.push_back(entry);
}

void TransactionLogger::log(int turnNumber, const std::string& actionType,
                            const Player& player,
                            const std::string& description) {
  data::LogEntry entry;
  entry.turnNumber = turnNumber;
  entry.playerName = player.getName();
  entry.actionType = actionType;
  entry.description = description;
  entries_.push_back(entry);
}

const std::vector<data::LogEntry>& TransactionLogger::getLog() const {
  return entries_;
}

std::vector<data::LogEntry> TransactionLogger::getRecentLog(int n) const {
  if (n <= 0 || n >= static_cast<int>(entries_.size()))
    return std::vector<data::LogEntry>(entries_.begin(), entries_.end());

  return std::vector<data::LogEntry>(entries_.end() - n, entries_.end());
}

void TransactionLogger::clear() { entries_.clear(); }
}  // namespace logic