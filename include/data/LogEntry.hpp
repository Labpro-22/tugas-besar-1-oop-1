#pragma once
#include <string>

class LogEntry {
public:
    int         turnNumber  = 0;
    std::string playerName;
    std::string actionType; // mis: "BELI", "DADU", "SEWA"
    std::string description;
};
