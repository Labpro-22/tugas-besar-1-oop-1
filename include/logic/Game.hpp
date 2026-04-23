#pragma once

#include "logic/Bank.hpp"
#include "logic/Board.hpp"
#include "core/GameContext.hpp"
#include <string>
#include <utility>
#include <vector>
#include <random>

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

class Game : public core::GameContext{
private:
  Board board_;
  std::vector<core::Player *> players_;
  Bank bank_;
  TransactionLogger *logger_;

  int currentPlayerId_;
  GameState state_;
  std::pair<int, int> lastDiceRoll_;
  int turnCount_;
	std::mt19937 rng_;
	int doubles_; 
	bool hasExtraTurn_; 
public:
  Game(std::vector<core::Player *> players, TransactionLogger *logger);
  ~Game() = default;

  void startGame();
  void nextTurn();
  bool checkWinCondition() const;

  std::pair<int, int> getLastDiceRoll() const override;
  int getTurnCount() const;

  void setDice(int d1, int d2);

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

  void offerProperty(core::Player& p, core::Property& prop) override;

  void chargeRent(core::Player& p, core::Property& prop) override;

  void sendToJail(core::Player& p) override;

  void chargeTax(core::Player& p, int rate, bool isPercentage) override;

  void activateFestival(core::Player& p) override;

  void drawChanceCard(core::Player& p) override;

  void drawCommunityChestCard(core::Player& p) override;

  void payPlayerFromBank(core::Player& p, int amount) override;

  int getGoSalary() const override;

  void logEvent(const std::string &action, core::Player &p, int value);

  void logEvent(const std::string &action, core::Player &p, core::Property &prop, int value);

  core::Player *getCurrentPlayer() const;
  GameState getState() const;
};

} // namespace logic
