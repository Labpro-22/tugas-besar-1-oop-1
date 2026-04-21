#include "data/SaveLoadManager.hpp"

#include <fstream>
#include <map>
#include <sstream>

#include "core/GameException.hpp"

namespace data {
// Helper functions
static std::string trimCR(std::string s) {
  while (!s.empty() && (s.back() == '\r' || s.back() == ' ')) s.pop_back();
  return s;
}

static std::string encodePlayerStatus(const PlayerDTO& p) {
  if (p.isBankrupt) return "BANKRUPT";
  if (p.jailTurns > 0) return "JAILED_" + std::to_string(p.jailTurns);
  return "ACTIVE";
}

static void decodePlayerStatus(const std::string& status, PlayerDTO& p) {
  if (status == "BANKRUPT") {
    p.isBankrupt = true;
    p.jailTurns = 0;
  } else if (status.rfind("JAILED", 0) == 0) {
    p.isBankrupt = false;
    p.jailTurns = (status.size() > 7) ? std::stoi(status.substr(7)) : 0;
  } else {
    p.isBankrupt = false;
    p.jailTurns = 0;
  }
}

void SaveLoadManager::save(const GameStateDTO& dto,
                           const std::string& filename) const {
  std::ofstream file(filename);
  if (!file.is_open()) throw FileIOException{filename, "write"};

  // Turn info
  file << dto.turnCount << " " << dto.maxTurn << "\n";

  // Players
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

  // Turn order
  for (size_t i = 0; i < dto.turnOrder.size(); ++i) {
    if (i > 0) file << " ";
    file << dto.turnOrder[i];
  }
  file << "\n";

  // Active player
  file << dto.activePlayerName << "\n";

  // Property state
  file << dto.properties.size() << "\n";
  for (const auto* prop : dto.properties) {
    std::string owner = prop->ownerName.empty() ? "BANK" : prop->ownerName;

    std::string propStatus;
    if (prop->ownerName.empty())
      propStatus = "BANK";
    else if (prop->isMortgaged)
      propStatus = "MORTGAGED";
    else
      propStatus = "OWNED";

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

  // Skill deck
  file << dto.skillDeckOrder.size() << "\n";
  for (const auto& card : dto.skillDeckOrder) file << card << "\n";

  // Transaction log
  file << dto.log.size() << "\n";
  for (const auto& entry : dto.log) {
    file << entry.turnNumber << " " << entry.playerName << " "
         << entry.actionType << " " << entry.description << "\n";
  }
}

GameStateDTO SaveLoadManager::load(const std::string& filename) const {
  std::ifstream file(filename);
  if (!file.is_open()) throw FileIOException{filename, "read"};

  GameStateDTO dto;
  std::string line;

  // Turn info
  if (!std::getline(file, line))
    throw InvalidConfigException{filename, "turn info on line 1"};
  {
    std::istringstream ss(trimCR(line));
    ss >> dto.turnCount >> dto.maxTurn;
    if (ss.fail())
      throw InvalidConfigException{filename, "TURN_SAAT_INI MAX_TURN integers"};
  }

  // Players
  int numPlayers = 0;
  file >> numPlayers;
  file.ignore();

  if (numPlayers < 2 || numPlayers > 4)
    throw InvalidConfigException{filename, "player count between 2 and 4"};

  for (int i = 0; i < numPlayers; ++i) {
    if (!std::getline(file, line))
      throw InvalidConfigException{filename,
                                   "player state row " + std::to_string(i + 1)};

    PlayerDTO player;
    std::istringstream ss(trimCR(line));
    std::string status;
    int isCOM = 0;
    ss >> player.name >> player.balance >> player.positionCode >> status >>
        isCOM;

    if (ss.fail())
      throw InvalidConfigException{
          filename, "player row: NAME BALANCE POSITION STATUS IS_COM"};

    decodePlayerStatus(status, player);
    player.isComputer = (isCOM == 1);

    int numCards = 0;
    file >> numCards;
    file.ignore();

    for (int j = 0; j < numCards; ++j) {
      if (!std::getline(file, line))
        throw InvalidConfigException{filename,
                                     "card row for player " + player.name};

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

  // Turn order
  if (!std::getline(file, line))
    throw InvalidConfigException{filename, "turn order line"};
  {
    std::istringstream ss(trimCR(line));
    std::string name;
    while (ss >> name) dto.turnOrder.push_back(name);
  }

  // Active player
  if (!std::getline(file, dto.activePlayerName))
    throw InvalidConfigException{filename, "active player line"};
  dto.activePlayerName = trimCR(dto.activePlayerName);

  // load currentPlayerIdx dari turnOrder
  dto.currentPlayerIdx = 0;
  for (int i = 0; i < static_cast<int>(dto.turnOrder.size()); ++i) {
    if (dto.turnOrder[i] == dto.activePlayerName) {
      dto.currentPlayerIdx = i;
      break;
    }
  }

  // Property state
  int numProperties = 0;
  file >> numProperties;
  file.ignore();

  // Track owner untuk populate ownedPropertyIds
  std::map<std::string, std::vector<int>> ownerToIndices;

  for (int i = 0; i < numProperties; ++i) {
    if (!std::getline(file, line))
      throw InvalidConfigException{filename,
                                   "property row " + std::to_string(i + 1)};

    std::istringstream ss(trimCR(line));
    std::string code, type, owner, propStatus, nBuildings;
    int fmult = 1, fdur = 0;
    ss >> code >> type >> owner >> propStatus >> fmult >> fdur >> nBuildings;

    if (ss.fail())
      throw InvalidConfigException{
          filename,
          "property row: CODE TYPE OWNER STATUS FMULT FDUR N_BANGUNAN"};

    PropertyDTO* prop = nullptr;

    if (type == "street") {
      auto* sp = new StreetPropertyDTO();
      sp->festMult = fmult;
      sp->festDur = fdur;
      sp->level = (nBuildings == "H") ? 5 : std::stoi(nBuildings);
      prop = sp;
    } else if (type == "railroad") {
      prop = new RailroadPropertyDTO();
    } else if (type == "utility") {
      prop = new UtilityPropertyDTO();
    } else {
      throw InvalidConfigException{
          filename,
          "property type: street|railroad|utility, got '" + type + "'"};
    }

    prop->code = code;
    prop->id = -1;  // caller resolve via code + Board
    prop->ownerName = (owner == "BANK") ? "" : owner;
    prop->isMortgaged = (propStatus == "MORTGAGED");

    if (!prop->ownerName.empty()) ownerToIndices[prop->ownerName].push_back(i);

    dto.properties.push_back(prop);
  }

  // Populate ownedPropertyIds per player
  for (auto& player : dto.players) {
    auto it = ownerToIndices.find(player.name);
    if (it != ownerToIndices.end()) player.ownedPropertyIds = it->second;
  }

  // Skill deck
  int numDeckCards = 0;
  file >> numDeckCards;
  file.ignore();

  for (int i = 0; i < numDeckCards; ++i) {
    if (!std::getline(file, line))
      throw InvalidConfigException{filename,
                                   "skill deck card " + std::to_string(i + 1)};
    dto.skillDeckOrder.push_back(trimCR(line));
  }

  // Transaction log
  int numLogEntries = 0;
  file >> numLogEntries;
  file.ignore();

  for (int i = 0; i < numLogEntries; ++i) {
    if (!std::getline(file, line))
      throw InvalidConfigException{filename,
                                   "log entry " + std::to_string(i + 1)};

    std::istringstream ss(trimCR(line));
    LogEntry entry;
    ss >> entry.turnNumber >> entry.playerName >> entry.actionType;
    std::getline(ss, entry.description);
    if (!entry.description.empty() && entry.description[0] == ' ')
      entry.description = entry.description.substr(1);
    dto.log.push_back(entry);
  }

  return dto;
}
}  // namespace data
