#include "data/LogSerializer.hpp"
#include <fstream>
#include <sstream>
#include "core/GameException.hpp"

namespace data {

static std::string escapeDescription(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (char c : s) {
        if      (c == '\\') out += "\\\\";
        else if (c == '\n') out += "\\n";
        else                out += c;
    }
    return out;
}

static std::string unescapeDescription(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\\' && i + 1 < s.size()) {
            ++i;
            if      (s[i] == 'n')  out += '\n';
            else if (s[i] == '\\') out += '\\';
            else { out += '\\'; out += s[i]; }
        } else {
            out += s[i];
        }
    }
    return out;
}

void LogSerializer::writeLog(const std::vector<LogEntry>& entries,
                             const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) throw FileIOException{filename, "write"};

    file << entries.size() << "\n";
    for (const auto& e : entries) {
        file << e.turnNumber << " " << e.playerName << " "
             << e.actionType << " " << escapeDescription(e.description) << "\n";
    }
}

std::vector<LogEntry> LogSerializer::readLog(const std::string& filename) const {
    std::ifstream file(filename);
    if (!file.is_open()) throw FileIOException{filename, "read"};

    std::vector<LogEntry> entries;
    std::string line;
    if (!std::getline(file, line))
        throw InvalidConfigException{filename, "entry count on first line"};

    int count = 0;
    try { count = std::stoi(line); }
    catch (...) { throw InvalidConfigException{filename, "integer entry count"}; }

    entries.reserve(count);
    for (int i = 0; i < count; ++i) {
        if (!std::getline(file, line))
            throw FileIOException{filename,
                "read, unexpected EOF at entry " + std::to_string(i + 1)};

        std::istringstream ss(line);
        LogEntry e;
        ss >> e.turnNumber >> e.playerName >> e.actionType;
        std::getline(ss, e.description);
        if (!e.description.empty() && e.description[0] == ' ')
            e.description = e.description.substr(1);

        e.description = unescapeDescription(e.description);
        entries.push_back(e);
    }
    return entries;
}

} // namespace data
