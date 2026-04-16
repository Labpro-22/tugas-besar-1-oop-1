#include "data/LogSerializer.hpp"
#include "core/GameException.hpp"
#include <fstream>
#include <sstream>

// Log file format
//   <JUMLAH_ENTRI>
//   <TURN> <PLAYERNAME> <ACTION_TYPE> <DESCRIPTION...>
//   ...

void LogSerializer::writeLog(const std::vector<LogEntry>& entries,
                             const std::string&           filename) const {
    std::ofstream file(filename);
    if (!file.is_open())
        throw FileIOException{filename, "write"};

    file << entries.size() << "\n";
    for (const auto& e : entries) {
        file << e.turnNumber  << " "
             << e.playerName  << " "
             << e.actionType  << " "
             << e.description << "\n";
    }
}

std::vector<LogEntry> LogSerializer::readLog(const std::string& filename) const {
    std::ifstream file(filename);
    if (!file.is_open())
        throw FileIOException{filename, "read"};

    std::vector<LogEntry> entries;
    std::string           line;

    if (!std::getline(file, line))
        throw InvalidConfigException{filename, "entry count on first line"};

    int count = 0;
    try { count = std::stoi(line); }
    catch (...) {
        throw InvalidConfigException{filename, "integer entry count"};
    }

    entries.reserve(count);
    for (int i = 0; i < count; ++i) {
        if (!std::getline(file, line))
            throw FileIOException{
                filename,
                "read, unexpected EOF at entry " + std::to_string(i + 1)};

        std::istringstream ss(line);
        LogEntry e;
        ss >> e.turnNumber >> e.playerName >> e.actionType;
        std::getline(ss, e.description);
        if (!e.description.empty() && e.description[0] == ' ')
            e.description = e.description.substr(1);

        entries.push_back(e);
    }

    return entries;
}
