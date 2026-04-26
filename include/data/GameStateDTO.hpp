#pragma once
#include <string>
#include <vector>

#include "LogEntry.hpp"

namespace data {

class HeldCardDTO {
public:
    std::string cardType;
    int value = 0;
    int remainingDuration = 0;
};

class PlayerDTO {
public:
    std::string name;
    int balance = 0;
    std::string positionCode;
    int jailTurns = 0;
    bool isBankrupt = false;
    bool isComputer = false;

    std::vector<int> ownedPropertyIds;  // derived dari PropertyDTO.ownerName saat load
    std::vector<HeldCardDTO> heldCards;
};

class PropertyDTO {
public:
    int id = -1;
    std::string code;
    std::string ownerName;
    bool isMortgaged = false;

    virtual ~PropertyDTO() = default;
    virtual std::string getType() const = 0;
};

class StreetPropertyDTO : public PropertyDTO {
public:
    int level = 0;     // 0-4 = rumah; 5 = hotel
    int festMult = 1;  // 1|2|4|8
    int festDur = 0;   // 0-3

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

class GameStateDTO {
public:
    // Dynamic Board
    int boardSize = 40;  // 20|24|28|...|60. disimpan di baris pertama save file

    // Game meta
    int turnCount = 0;
    int maxTurn = 0;
    int currentPlayerIdx = 0;

    std::vector<PlayerDTO> players;
    std::vector<std::string> turnOrder;
    std::string activePlayerName;

    std::vector<PropertyDTO*> properties; // owned pointers
    std::vector<std::string> skillDeckOrder;
    std::vector<LogEntry> log;

    GameStateDTO() = default;

    ~GameStateDTO() {
        for (auto* p : properties) delete p;
    }

    GameStateDTO(const GameStateDTO&) = delete;
    GameStateDTO& operator=(const GameStateDTO&) = delete;

    GameStateDTO(GameStateDTO&& o) noexcept
        : boardSize(o.boardSize),
          turnCount(o.turnCount), maxTurn(o.maxTurn),
          currentPlayerIdx(o.currentPlayerIdx),
          players(std::move(o.players)),
          turnOrder(std::move(o.turnOrder)),
          activePlayerName(std::move(o.activePlayerName)),
          properties(std::move(o.properties)),
          skillDeckOrder(std::move(o.skillDeckOrder)),
          log(std::move(o.log))
    {
        o.properties.clear();
    }

    GameStateDTO& operator=(GameStateDTO&& o) noexcept {
        if (this != &o) {
            for (auto* p : properties) delete p;
            boardSize       = o.boardSize;
            turnCount       = o.turnCount;
            maxTurn         = o.maxTurn;
            currentPlayerIdx = o.currentPlayerIdx;
            players         = std::move(o.players);
            turnOrder       = std::move(o.turnOrder);
            activePlayerName = std::move(o.activePlayerName);
            properties      = std::move(o.properties);
            skillDeckOrder  = std::move(o.skillDeckOrder);
            log             = std::move(o.log);
            o.properties.clear();
        }
        return *this;
    }
};

}  // namespace data