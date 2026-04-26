#include "data/SaveLoadManager.hpp"

#include <fstream>
#include <map>
#include <memory>
#include <sstream>

#include "core/GameException.hpp"
#include "data/TransactionLogger.hpp"

namespace data {
namespace {

std::string trimCR(std::string s) {
  while (!s.empty() && (s.back() == '\r' || s.back() == ' ')) s.pop_back();
  return s;
}

std::string encodePlayerStatus(const PlayerDTO& p) {
  if (p.isBankrupt) return "BANKRUPT";
  if (p.jailTurns > 0) return "JAILED_" + std::to_string(p.jailTurns);
  return "ACTIVE";
}

void decodePlayerStatus(const std::string& status, PlayerDTO& p) {
  if (status == "BANKRUPT") {
    p.isBankrupt = true;
    p.jailTurns = 0;
  } else if (status.rfind("JAILED_", 0) == 0 && status.size() > 7) {
    p.isBankrupt = false;
    p.jailTurns = std::stoi(status.substr(7));
  } else {
    p.isBankrupt = false;
    p.jailTurns = 0;
  }
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
      return "BANGKRUT";
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

void SaveLoadManager::save(const GameStateDTO& dto,
                           const std::string& filename) const {
  TransactionLogger::get().log(dto.turnCount, dto.activePlayerName,
                               LogAction::GAME_SAVE,
                               "State saved to " + filename);

  std::ofstream file(filename);
  if (!file.is_open()) throw FileIOException{filename, "write"};

  file << dto.boardSize << "\n";
  file << dto.turnCount << " " << dto.maxTurn << "\n";

  file << dto.players.size() << "\n";
  for (const auto& p : dto.players) {
    file << p.name << " " << p.balance << " " << p.positionCode << " "
         << encodePlayerStatus(p) << " " << (p.isComputer ? 1 : 0) << "\n";
    file << p.heldCards.size() << "\n";
    for (const auto& card : p.heldCards) {
      file << card.cardType;
      if (card.value != 0) file << " " << card.value;
      if (card.remainingDuration != 0) file << " " << card.remainingDuration;
      file << "\n";
    }
  }

  for (size_t i = 0; i < dto.turnOrder.size(); ++i) {
    if (i > 0) file << " ";
    file << dto.turnOrder[i];
  }
  file << "\n";
  file << dto.activePlayerName << "\n";

  file << dto.properties.size() << "\n";
  for (const auto& propPtr : dto.properties) {
    const auto* prop = propPtr.get();
    std::string owner = prop->ownerName.empty() ? "BANK" : prop->ownerName;
    std::string propStatus;
    if (prop->ownerName.empty()) {
      propStatus = "BANK";
    } else if (prop->isMortgaged) {
      propStatus = "MORTGAGED";
    } else {
      propStatus = "OWNED";
    }

    int fmult = 1;
    int fdur = 0;
    std::string nBuildings = "0";

    if (prop->getType() == "street") {
      const auto* sp = static_cast<const StreetPropertyDTO*>(prop);
      fmult = sp->festMult;
      fdur = sp->festDur;
      nBuildings = (sp->level == 5) ? "H" : std::to_string(sp->level);
    }

    file << prop->code << " " << prop->getType() << " " << owner << " "
         << propStatus << " " << fmult << " " << fdur << " " << nBuildings
         << "\n";
  }

  file << dto.skillDeckOrder.size() << "\n";
  for (const auto& card : dto.skillDeckOrder) file << card << "\n";

  const auto& logs = TransactionLogger::get().getEntries();
  file << logs.size() << "\n";
  for (const auto& entry : logs) {
    file << entry.getTurnNumber() << " " << entry.getPlayerName() << " "
         << actionToString(entry.getActionType()) << " "
         << entry.getDescription() << "\n";
  }
}

GameStateDTO SaveLoadManager::load(const std::string& filename) const {
  std::ifstream file(filename);
  if (!file.is_open()) throw FileIOException{filename, "read"};

  GameStateDTO dto;
  std::string line;

  if (!std::getline(file, line)) {
    throw InvalidConfigException{filename, "board size on line 1"};
  }
  {
    std::istringstream ss(trimCR(line));
    ss >> dto.boardSize;
    if (ss.fail()) throw InvalidConfigException{filename, "integer board size"};
  }

  if (!std::getline(file, line)) {
    throw InvalidConfigException{filename, "turn info on line 2"};
  }
  {
    std::istringstream ss(trimCR(line));
    ss >> dto.turnCount >> dto.maxTurn;
    if (ss.fail()) {
      throw InvalidConfigException{filename, "TURN_SAAT_INI MAX_TURN integers"};
    }
  }

  int numPlayers = 0;
  file >> numPlayers;
  file.ignore();

  if (numPlayers < 2 || numPlayers > 4) {
    throw InvalidConfigException{filename, "player count between 2 and 4"};
  }

  for (int i = 0; i < numPlayers; ++i) {
    if (!std::getline(file, line)) {
      throw InvalidConfigException{filename,
                                   "player state row " + std::to_string(i + 1)};
    }

    PlayerDTO player;
    std::istringstream ss(trimCR(line));
    std::string status;
    int isCOM = 0;
    ss >> player.name >> player.balance >> player.positionCode >> status >>
        isCOM;
    if (ss.fail()) {
      throw InvalidConfigException{
          filename, "player row: NAME BALANCE POSITION STATUS IS_COM"};
    }

    decodePlayerStatus(status, player);
    player.isComputer = (isCOM == 1);

    int numCards = 0;
    file >> numCards;
    file.ignore();

    for (int j = 0; j < numCards; ++j) {
      if (!std::getline(file, line)) {
        throw InvalidConfigException{filename,
                                     "card row for player " + player.name};
      }

      HeldCardDTO card;
      std::istringstream css(trimCR(line));
      css >> card.cardType;
      if (!(css >> card.value)) card.value = 0;
      if (card.cardType == "DiscountCard") {
        if (!(css >> card.remainingDuration)) card.remainingDuration = 0;
      }
      player.heldCards.push_back(card);
    }
    dto.players.push_back(std::move(player));
  }

  if (!std::getline(file, line)) {
    throw InvalidConfigException{filename, "turn order line"};
  }
  {
    std::istringstream ss(trimCR(line));
    std::string name;
    while (ss >> name) dto.turnOrder.push_back(name);
  }

  if (!std::getline(file, dto.activePlayerName)) {
    throw InvalidConfigException{filename, "active player line"};
  }
  dto.activePlayerName = trimCR(dto.activePlayerName);

  dto.currentPlayerIdx = 0;
  for (int i = 0; i < static_cast<int>(dto.turnOrder.size()); ++i) {
    if (dto.turnOrder[i] == dto.activePlayerName) {
      dto.currentPlayerIdx = i;
      break;
    }
  }

  int numProperties = 0;
  file >> numProperties;
  file.ignore();

  std::map<std::string, std::vector<int>> ownerToIndices;

  for (int i = 0; i < numProperties; ++i) {
    if (!std::getline(file, line)) {
      throw InvalidConfigException{filename,
                                   "property row " + std::to_string(i + 1)};
    }

    std::istringstream ss(trimCR(line));
    std::string code, type, owner, propStatus, nBuildings;
    int fmult = 1, fdur = 0;
    ss >> code >> type >> owner >> propStatus >> fmult >> fdur >> nBuildings;

    if (ss.fail()) {
      throw InvalidConfigException{
          filename,
          "property row: CODE TYPE OWNER STATUS FMULT FDUR N_BANGUNAN"};
    }

    std::unique_ptr<PropertyDTO> prop;
    if (type == "street") {
      auto sp = std::make_unique<StreetPropertyDTO>();
      sp->festMult = fmult;
      sp->festDur = fdur;
      sp->level = (nBuildings == "H") ? 5 : std::stoi(nBuildings);
      prop = std::move(sp);
    } else if (type == "railroad") {
      prop = std::make_unique<RailroadPropertyDTO>();
    } else if (type == "utility") {
      prop = std::make_unique<UtilityPropertyDTO>();
    } else {
      throw InvalidConfigException{
          filename,
          "property type: street|railroad|utility, got '" + type + "'"};
    }

    prop->code = code;
    prop->id = -1;
    prop->ownerName = (owner == "BANK") ? "" : owner;
    prop->isMortgaged = (propStatus == "MORTGAGED");

    if (!prop->ownerName.empty()) ownerToIndices[prop->ownerName].push_back(i);

    dto.properties.push_back(std::move(prop));
  }

  for (auto& player : dto.players) {
    auto it = ownerToIndices.find(player.name);
    if (it != ownerToIndices.end()) player.ownedPropertyIds = it->second;
  }

  int numDeckCards = 0;
  file >> numDeckCards;
  file.ignore();

  for (int i = 0; i < numDeckCards; ++i) {
    if (!std::getline(file, line)) {
      throw InvalidConfigException{filename,
                                   "skill deck card " + std::to_string(i + 1)};
    }
    dto.skillDeckOrder.push_back(trimCR(line));
  }

  int numLogEntries = 0;
  file >> numLogEntries;
  file.ignore();

  std::vector<LogEntry> loadedLogs;
  loadedLogs.reserve(numLogEntries);

  for (int i = 0; i < numLogEntries; ++i) {
    if (!std::getline(file, line)) {
      throw InvalidConfigException{filename,
                                   "log entry " + std::to_string(i + 1)};
    }

    std::istringstream ss(trimCR(line));
    int turn = 0;
    std::string playerName;
    std::string actionName;
    ss >> turn >> playerName >> actionName;

    std::string description;
    std::getline(ss, description);
    if (!description.empty() && description[0] == ' ') {
      description = description.substr(1);
    }

    LogEntry entry(turn, playerName, actionFromString(actionName), description);
    loadedLogs.push_back(entry);
    dto.log.push_back(entry);
  }

  TransactionLogger::get().restore(loadedLogs);
  TransactionLogger::get().log(dto.turnCount, dto.activePlayerName,
                               LogAction::GAME_LOAD,
                               "State loaded from " + filename);

  return dto;
}

}  // namespace data
