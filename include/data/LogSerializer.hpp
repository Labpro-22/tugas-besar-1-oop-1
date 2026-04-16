#pragma once
#include <string>
#include <vector>
#include "data/LogEntry.hpp"

class LogSerializer {
public:
    // tulis semua entries ke filename
    void writeLog(const std::vector<LogEntry>& entries,
                  const std::string& filename) const;

    // baca semua entries dari filename
    std::vector<LogEntry> readLog(const std::string& filename) const;
};
