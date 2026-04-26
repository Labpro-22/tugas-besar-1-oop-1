#include "data/TransactionLogger.hpp"

#include <fstream>
#include <sstream>

#include "core/GameException.hpp"

namespace data {
namespace {

std::string escapeDescription(const std::string& s) {
  std::string out;
  out.reserve(s.size());
  for (char c : s) {
    if (c == '\\') {
      out += "\\\\";
    } else if (c == '\n') {
      out += "\\n";
    } else {
      out += c;
    }
  }
  return out;
}

std::string unescapeDescription(const std::string& s) {
  std::string out;
  out.reserve(s.size());
  for (std::size_t i = 0; i < s.size(); ++i) {
    if (s[i] == '\\' && i + 1 < s.size()) {
      ++i;
      if (s[i] == 'n') {
        out += '\n';
      } else if (s[i] == '\\') {
        out += '\\';
      } else {
        out += '\\';
        out += s[i];
      }
    } else {
      out += s[i];
    }
  }
  return out;
}

std::string actionToString(LogAction action) {
  switch (action) {
    case LogAction::DICE_ROLL:
      return "LEMPAR_DADU";
    case LogAction::PIECE_MOVEMENT:
      return "BIDAK_GERAK";
    case LogAction::PROPERTY_PURCHASE:
      return "PEMBELIAN_PROPERTI";
    case LogAction::RENT_PAYMENT:
      return "PEMBAYARAN_SEWA";
    case LogAction::TAX_PAYMENT:
      return "PEMBAYARAN_PAJAK";
    case LogAction::BUILD_HOUSE:
      return "BANGUN_RUMAH";
    case LogAction::SALE_HOUSE:
      return "JUAL_RUMAH";
    case LogAction::MORTGAGE:
      return "GADAI";
    case LogAction::UNMORTGAGE:
      return "TEBUS";
    case LogAction::SPECIAL_CARD_USE:
      return "MENGGUNAKAN_KARTU_SPESIAL";
    case LogAction::CHANCE_CARD_DRAW:
      return "MENGAMBIL_KARTU_KESEMPATAN";
    case LogAction::COMMUNITY_CARD_DRAW:
      return "MENGAMBIL_KARTU_DANA_UMUM";
    case LogAction::AUCTION_BID:
      return "MENAWAR_LELANG";
    case LogAction::AUCTION_RESULT:
      return "HASIL_LELANG";
    case LogAction::FESTIVAL_ACTIVATION:
      return "AKTIVASI_FESTIVAL";
    case LogAction::BANKRUPTCY:
      return "BANKRUPTCY";
    case LogAction::GAME_SAVE:
      return "GAME_SAVE";
    case LogAction::GAME_LOAD:
      return "GAME_LOAD";
  }
  return "INVALID";
}

LogAction actionFromString(const std::string& value) {
  if (value == "LEMPAR_DADU") return LogAction::DICE_ROLL;
  if (value == "BIDAK_GERAK") return LogAction::PIECE_MOVEMENT;
  if (value == "PEMBELIAN_PROPERTI") return LogAction::PROPERTY_PURCHASE;
  if (value == "PEMBAYARAN_SEWA") return LogAction::RENT_PAYMENT;
  if (value == "PEMBAYARAN_PAJAK") return LogAction::TAX_PAYMENT;
  if (value == "BANGUN_RUMAH") return LogAction::BUILD_HOUSE;
  if (value == "JUAL_RUMAH") return LogAction::SALE_HOUSE;
  if (value == "GADAI") return LogAction::MORTGAGE;
  if (value == "TEBUS") return LogAction::UNMORTGAGE;
  if (value == "MENGGUNAKAN_KARTU_SPESIAL") return LogAction::SPECIAL_CARD_USE;
  if (value == "MENGAMBIL_KARTU_KESEMPATAN") return LogAction::CHANCE_CARD_DRAW;
  if (value == "MENGAMBIL_KARTU_DANA_UMUM") {
    return LogAction::COMMUNITY_CARD_DRAW;
  }
  if (value == "MENAWAR_LELANG") return LogAction::AUCTION_BID;
  if (value == "HASIL_LELANG") return LogAction::AUCTION_RESULT;
  if (value == "AKTIVASI_FESTIVAL") {
    return LogAction::FESTIVAL_ACTIVATION;
  }
  if (value == "BANGKRUT") return LogAction::BANKRUPTCY;
  if (value == "GAME_SAVE") return LogAction::GAME_SAVE;
  if (value == "GAME_LOAD") return LogAction::GAME_LOAD;

  throw InvalidConfigException("log action", "known action enum string");
}

}  // namespace

void TransactionLogger::log(int turnNumber, const std::string& playerName,
                            LogAction actionType,
                            const std::string& description) {
  entries_.emplace_back(turnNumber, playerName, actionType, description);
}

void TransactionLogger::serialize(const std::string& filename) const {
  std::ofstream file(filename);
  if (!file.is_open()) {
    throw FileIOException{filename, "write"};
  }

  file << entries_.size() << "\n";
  for (const auto& entry : entries_) {
    file << entry.getTurnNumber() << " " << entry.getPlayerName() << " "
         << actionToString(entry.getActionType()) << " "
         << escapeDescription(entry.getDescription()) << "\n";
  }
}

void TransactionLogger::deserialize(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw FileIOException{filename, "read"};
  }

  std::vector<LogEntry> loaded;
  std::string line;

  if (!std::getline(file, line)) {
    throw InvalidConfigException{filename, "entry count on first line"};
  }

  int count = 0;
  try {
    count = std::stoi(line);
  } catch (...) {
    throw InvalidConfigException{filename, "integer entry count"};
  }

  loaded.reserve(count);
  for (int i = 0; i < count; ++i) {
    if (!std::getline(file, line)) {
      throw FileIOException{
          filename, "read, unexpected EOF at entry " + std::to_string(i + 1)};
    }

    std::istringstream ss(line);
    int turn = 0;
    std::string playerName;
    std::string actionName;
    ss >> turn >> playerName >> actionName;

    std::string description;
    std::getline(ss, description);
    if (!description.empty() && description[0] == ' ') {
      description.erase(description.begin());
    }
    description = unescapeDescription(description);

    loaded.emplace_back(turn, playerName, actionFromString(actionName),
                        description);
  }

  entries_ = std::move(loaded);
}

const std::vector<LogEntry>& TransactionLogger::getEntries() const {
  return entries_;
}

void TransactionLogger::restore(const std::vector<LogEntry>& entries) {
  entries_ = entries;
}

void TransactionLogger::clear() { entries_.clear(); }

}  // namespace data
