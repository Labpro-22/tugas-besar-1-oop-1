#pragma once

#include <string>
#include <vector>

#include "data/LogEntry.hpp"

namespace data {

class TransactionLogger {
 public:
  TransactionLogger(const TransactionLogger&) = delete;
  TransactionLogger& operator=(const TransactionLogger&) = delete;
  TransactionLogger(TransactionLogger&&) = delete;
  TransactionLogger& operator=(TransactionLogger&&) = delete;

  static TransactionLogger& get() {
    static TransactionLogger instance;
    return instance;
  }

  void log(int turnNumber, const std::string& playerName, LogAction actionType,
           const std::string& description);

  void serialize(const std::string& filename) const;
  void deserialize(const std::string& filename);

  const std::vector<LogEntry>& getEntries() const;
  void restore(const std::vector<LogEntry>& entries);
  void clear();

 private:
  TransactionLogger() = default;

  std::vector<LogEntry> entries_;
};

}  // namespace data
