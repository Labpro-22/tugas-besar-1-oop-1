#pragma once
#include <string>
#include <vector>

#include "data/LogEntry.hpp"

namespace data {
class HeldCardDTO {
 public:
  std::string cardType;  // "MoveCard", "ShieldCard", "DiscountCard", etc.
  int value = 0;         // steps (MoveCard) or percent (DiscountCard); 0 if N/A
  int remainingDuration = 0;  // turns remaining; only non-zero for DiscountCard
};

class PlayerDTO {
 public:
  std::string name;
  int balance = 0;
  std::string
      positionCode;   // tile code, misal: "JKT" (diresolve ke index board)
  int jailTurns = 0;  // 0 = tidak di penjara; 1–3 = percobaan penjara
  bool isBankrupt = false;
  bool isComputer = false;

  std::vector<int>
      ownedPropertyIds;  // diisi setelah load() oleh SaveLoadManager
  std::vector<HeldCardDTO> heldCards;
};

class PropertyDTO {
 public:
  int id = -1;            // -1 setelah load; perlu diresolve via code + Board
  std::string code;       // misal: "JKT", "GBR"
  std::string ownerName;  // string kosong berarti properti milik BANK
  bool isMortgaged = false;

  virtual ~PropertyDTO() = default;
  virtual std::string getType() const = 0;
};

class StreetPropertyDTO : public PropertyDTO {
 public:
  int level = 0;     // 0–4 = jumlah rumah; 5 = hotel
  int festMult = 1;  // 1 | 2 | 4 | 8  (1 = ga aktif)
  int festDur = 0;   // 0–3 giliran tersisa (0 = ga aktif)

  std::string getType() const override { return "street"; }
};

class RailroadPropertyDTO : public PropertyDTO {
 public:
  std::string getType() const override { return "railroad"; }
};

class UtilityPropertyDTO : public PropertyDTO {
 public:
  std::string getType() const override { return "utility"; }
};

// Root transfer object antara Core dan Data layer
class GameStateDTO {
 public:
  int turnCount = 0;
  int maxTurn = 0;
  int currentPlayerIdx = 0;  // index ke turnOrder

  std::vector<PlayerDTO> players;
  std::vector<std::string> turnOrder;  // misal: ["Uname2", "Uname4", "Uname1"]
  std::string activePlayerName;

  std::vector<PropertyDTO*> properties;  // owned pointers

  std::vector<std::string> skillDeckOrder;  // sisa kartu di skill deck

  std::vector<LogEntry> log;

  // Ownership semantics
  GameStateDTO() = default;

  ~GameStateDTO() {
    for (auto* p : properties) delete p;
  }

  GameStateDTO(const GameStateDTO&) = delete;
  GameStateDTO& operator=(const GameStateDTO&) = delete;

  GameStateDTO(GameStateDTO&& o) noexcept
      : turnCount(o.turnCount),
        maxTurn(o.maxTurn),
        currentPlayerIdx(o.currentPlayerIdx),
        players(std::move(o.players)),
        turnOrder(std::move(o.turnOrder)),
        activePlayerName(std::move(o.activePlayerName)),
        properties(std::move(o.properties)),
        skillDeckOrder(std::move(o.skillDeckOrder)),
        log(std::move(o.log)) {
    o.properties.clear();
  }

  GameStateDTO& operator=(GameStateDTO&& o) noexcept {
    if (this != &o) {
      for (auto* p : properties) delete p;
      turnCount = o.turnCount;
      maxTurn = o.maxTurn;
      currentPlayerIdx = o.currentPlayerIdx;
      players = std::move(o.players);
      turnOrder = std::move(o.turnOrder);
      activePlayerName = std::move(o.activePlayerName);
      properties = std::move(o.properties);
      skillDeckOrder = std::move(o.skillDeckOrder);
      log = std::move(o.log);
      o.properties.clear();
    }
    return *this;
  }
};
}  // namespace data