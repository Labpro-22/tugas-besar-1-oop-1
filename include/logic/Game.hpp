#pragma once

#include "core/ActionCard.hpp"
#include "core/CardDeck.hpp"
#include "core/GameContext.hpp"
#include "data/LogEntry.hpp"
#include "logic/Bank.hpp"
#include "logic/Board.hpp"

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

  // Config-derived values — diisi oleh initialize()
  int goSalary_  = 200;
  int jailFine_  = 50;
  int maxTurn_   = 0;
  int boardSize_ = 40;
  std::string configPath_;
  
  // Card decks — diisi oleh initialize() setelah board terbangun
  core::CardDeck<core::ActionCard> chanceDeck_;
  core::CardDeck<core::ActionCard> communityChestDeck_;
  core::CardDeck<core::ActionCard> skillDeck_;
  
  void resolveFestival(core::Property* selectedProp);

  // Helpers untuk initialize()
  void buildChanceDeck();
  void buildCommunityChestDeck();
  void buildSkillDeck();
  
public:
  Game(std::vector<core::Player *> players, TransactionLogger *logger);
  ~Game() = default;

  /**
   * @brief Baca config, bangun board via DomainBuilder, dan populate semua deck.
   *
   * Dipanggil untuk New Game maupun Load Game (sebelum GameLoader::applyDTO).
   * Boleh dipanggil ulang — Board::clear() dijalankan di awal.
   *
   * @param boardSize Ukuran papan: 20, 24, 28, ..., 60.
   * @param configPath Path ke folder config (misal "config/").
   */
  void initialize(int boardSize, const std::string& configPath);
  void startGame(); 
  void setMediator(UIInputMediator* mediator);
  void nextTurn(); 
  bool checkWinCondition() const; 

  // Accessors
  std::pair<int, int> getLastDiceRoll() const override;
  int getTurnCount() const;
  int getMaxTurn() const;
  int getJailFine() const;
  int getBoardSize() const override;
  Board& getBoard();
  GameState getState() const;
  core::Player* getCurrentPlayer() const;
  const std::vector<core::Player*>& getPlayers() const override;

  // Dice
  void setDice(int d1, int d2);
  void rollDice();

  // Movement
  void moveCurrentPlayer();
  void handleTileAction(core::Tile* tile);

  // Property operations
  void buyProperty(core::Property* prop);
  void buildHouse(core::Player* buyer, core::Tile* at);
  void sellHouse(core::Player* seller, core::Tile* at);
  void mortgageProperty(core::Property* prop);
  void unmortgageProperty(core::Property* prop);
  void startAuction(core::Property* prop);

  // Card operations
  void giveCard(core::Player& player, core::ActionCard* card);

  // GameContext interface
  void offerProperty(core::Player& p, core::Property& prop) override;
  void chargeRent(core::Player& p, core::Property& prop) override;
  void sendToJail(core::Player& p) override;
  void chargeTax(core::Player& p, int flatRate, int percentageRate,
                  core::TaxType type) override;
  void activateFestival(core::Player& p) override;
  void drawChanceCard(core::Player& p) override;
  void drawCommunityChestCard(core::Player& p) override;
  void payPlayerFromBank(core::Player& p, int amount) override;
  int  getGoSalary() const override;

  void logEvent(const std::string& action, core::Player& p, int value) override;
  void logEvent(const std::string& action, core::Player& p,
                core::Property& prop, int value) override;

  // Setters untuk GameLoader
  core::Player* getPlayerByName(const std::string& name) const;
  void setTurnCount(int count);
  void setMaxTurn(int max);
  void setCurrentPlayerIdx(int idx);
  void setTurnOrder(const std::vector<std::string>& order);
  void restoreLog(const std::vector<data::LogEntry>& entries);

  /**
   * @brief Expose skill deck untuk GameLoader::restoreDeck().
   * Non-owning reference; lifetime mengikuti Game object.
   */
  core::CardDeck<core::ActionCard>& getSkillDeck();
};

} // namespace logic
