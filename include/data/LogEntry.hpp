#pragma once

#include <string>

namespace data {

enum class LogAction {
  DICE_ROLL,
  PIECE_MOVEMENT,
  PROPERTY_PURCHASE,
  RENT_PAYMENT,
  TAX_PAYMENT,
  BUILD_HOUSE,
  SALE_HOUSE,
  MORTGAGE,
  UNMORTGAGE,
  SPECIAL_CARD_USE,
  CHANCE_CARD_DRAW,
  COMMUNITY_CARD_DRAW,
  AUCTION_BID,
  AUCTION_RESULT,
  FESTIVAL_ACTIVATION,
  BANKRUPTCY,
  GAME_SAVE,
  GAME_LOAD
};

class LogEntry {
 public:
  LogEntry() = default;
  LogEntry(int turnNumber, std::string playerName, LogAction actionType,
           std::string description)
      : turnNumber_(turnNumber),
        playerName_(std::move(playerName)),
        actionType_(actionType),
        description_(std::move(description)) {}

  int getTurnNumber() const { return turnNumber_; }
  const std::string& getPlayerName() const { return playerName_; }
  LogAction getActionType() const { return actionType_; }
  const std::string& getDescription() const { return description_; }

  void setTurnNumber(int turnNumber) { turnNumber_ = turnNumber; }
  void setPlayerName(const std::string& playerName) {
    playerName_ = playerName;
  }
  void setActionType(LogAction actionType) { actionType_ = actionType; }
  void setDescription(const std::string& description) {
    description_ = description;
  }

 private:
  int turnNumber_ = 0;
  std::string playerName_;
  LogAction actionType_ = LogAction::PIECE_MOVEMENT;
  std::string description_;
};
}  // namespace data