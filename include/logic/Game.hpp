#pragma once

#include "logic/Bank.hpp"
#include "logic/Board.hpp"
#include <string>
#include <utility>
#include <vector>

namespace core {
class Player;
class Tile;
class Property;
} // namespace core

namespace logic {
class TransactionLogger;
}

namespace logic {

enum class GameState {
  PRE_ROLL,
  WAITING_FOR_DICE,
  TILE_ACTION,
  AUCTION,
  GAME_OVER
};

class Game {
private:
  Board board_;
  std::vector<core::Player *> players_;
  Bank bank_;
  TransactionLogger *logger_;

  int currentPlayerId_;
  GameState state_;
  std::pair<int, int> lastDiceRoll_;
  int turnCount_;

public:
  Game(std::vector<core::Player *> players, TransactionLogger *logger);
  ~Game() = default;

  void startGame();
  void nextTurn();
  bool checkWinCondition() const;

  Board &getBoard();

  void rollDice();
  void moveCurrentPlayer();
  void handleTileAction(core::Tile *tile);

  void buyProperty(core::Property *prop);
  void buildHouse(core::Player *buyer, core::Tile *at);
  void sellHouse(core::Player *seller, core::Tile *at);
  void mortgageProperty(core::Property *prop);
  void unmortgageProperty(core::Property *prop);

  void startAuction(core::Property *prop);

  void logEvent(const std::string &action, core::Player &p, int value);
  void logEvent(const std::string &action, core::Player &p,
                core::Property &prop, int value);

  core::Player *getCurrentPlayer() const;
  GameState getState() const;
};

} // namespace logic
