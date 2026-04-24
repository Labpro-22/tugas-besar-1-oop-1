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
class UIInputMediator;
} // namespace logic

namespace logic {

enum class GameState {
  PRE_ROLL,
  WAITING_FOR_DICE,
  TILE_ACTION,
  WAITING_FESTIVAL,
  WAITING_TAX,
  AUCTION,
  GAME_OVER
};

class Game : public core::GameContext{
private:
  Board board_;
  std::vector<core::Player *> players_;
  Bank bank_;
  TransactionLogger *logger_;
  UIInputMediator *mediator_;

  int currentPlayerId_;
  GameState state_;
  std::pair<int, int> lastDiceRoll_;
  int turnCount_;
 	std::mt19937 rng_;
	int doubles_; 
	bool hasExtraTurn_; 

  void resolveFestival(core::Property* selectedProp);

public:
  Game(std::vector<core::Player *> players, TransactionLogger *logger);
  ~Game() = default;

  void setMediator(UIInputMediator *mediator);

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

  void chargeTax(core::Player& p, int flatRate, int percentageRate, core::TaxType type) override;

  void activateFestival(core::Player& p) override;

  void drawChanceCard(core::Player& p) override;

  void drawCommunityChestCard(core::Player& p) override;

  void payPlayerFromBank(core::Player& p, int amount) override;

  int getGoSalary() const override;

  // NOTE: extension beyond M1 spec - see `core::GameContext` for rationale.
  const std::vector<core::Player*>& getPlayers() const override;

  // NOTE: extension beyond M1 spec - see `core::GameContext` for rationale.
  int getBoardSize() const override;

  void logEvent(const std::string &action, core::Player &p, int value);

  void logEvent(const std::string &action, core::Player &p, core::Property &prop, int value);

  core::Player *getCurrentPlayer() const;
  GameState getState() const;
};

} // namespace logic
