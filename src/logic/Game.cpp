#include "logic/Game.hpp"
#include "core/Player.hpp"
#include "core/Property.hpp"
#include "core/Tiles.hpp"
#include "core/GameException.hpp"
#include "logic/Auction.hpp"

#include <cstdlib>
#include <ctime>

namespace logic {

Game::Game(std::vector<core::Player*> players, TransactionLogger* logger)
    : players_(players), logger_(logger), currentPlayerId_(0), 
      state_(GameState::PRE_ROLL), lastDiceRoll_({0, 0}), turnCount_(1) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void Game::startGame() {
    state_ = GameState::PRE_ROLL;
}

Board& Game::getBoard(){
	return board_; 
} 

void Game::nextTurn() {
    if (checkWinCondition()) {
        state_ = GameState::GAME_OVER;
        return;
    }

    do {
        currentPlayerId_ = (currentPlayerId_ + 1) % players_.size();
        if (currentPlayerId_ == 0) {
            turnCount_++;
        }
    } while (players_[currentPlayerId_]->getIsBankrupt());

    state_ = GameState::PRE_ROLL;
}

bool Game::checkWinCondition() const {
    int activePlayers = 0;
    for (const auto* p : players_) {
        if (!p->getIsBankrupt()) {
            activePlayers++;
        }
    }
    return activePlayers <= 1;
}

void Game::rollDice() {
    if (state_ != GameState::PRE_ROLL && state_ != GameState::WAITING_FOR_DICE) {
        throw InvalidMoveException("state exception");
    }

    lastDiceRoll_.first = (std::rand() % 6) + 1;
    lastDiceRoll_.second = (std::rand() % 6) + 1;
}

void Game::moveCurrentPlayer() {
    core::Player* p = getCurrentPlayer();
    
    int steps = lastDiceRoll_.first + lastDiceRoll_.second;
    int boardSize = board_.getTileCount();
    
    if (boardSize == 0) {
        throw InvalidMoveException("board logging exception!");
    }

    int currentPos = p->getPosition();
    int newPos = (currentPos + steps) % boardSize;

    if (newPos < currentPos && newPos != 0) {
        bank_.pay(*p, 200); 
    }

    p->setPosition(newPos);
    core::Tile* landedTile = board_.getTile(newPos);
    
    handleTileAction(landedTile);
}

void Game::handleTileAction(core::Tile* tile) {
    if (!tile) throw InvalidMoveException("invalid tile.");
    
    state_ = GameState::TILE_ACTION;
    core::Player* p = getCurrentPlayer();
    
    tile->onLanded(*p, *this);
}

void Game::buyProperty(core::Property* prop) {
    core::Player* p = getCurrentPlayer();
    int price = prop->getPrice();

    if (prop->getOwner() != nullptr) {
        throw InvalidMoveException("owned property exception");
    }

    if (!p->canAfford(price)) {
        throw InsufficientFundsException(p->getBalance(), price);
    }

    *p -= price;
    bank_.receive(price);
    
    prop->setOwner(p);
    p->addProperty(prop);
}

void Game::buildHouse(core::Player* buyer, core::Tile* at) {
    core::Property* prop = at->getProperty();
    if (!prop) {
        throw InvalidMoveException("Bukan petak properti.");
    }
    
    if (prop->getType() != core::PropertyType::STREET) {
        throw InvalidMoveException("Hanya bisa membangun di properti tipe Street.");
    }

    core::Street* street = static_cast<core::Street*>(prop);

    if (street->getOwner() != buyer) {
        throw InvalidMoveException("Anda bukan pemilik properti ini.");
    }

    int cost = street->getHouseCost();
    if (!buyer->canAfford(cost)) {
        throw InsufficientFundsException(buyer->getBalance(), cost);
    }

    bank_.sellHouse(1);

    *buyer -= cost;
    bank_.receive(cost);
    street->build();
}

void Game::sellHouse(core::Player* seller, core::Tile* at) {
    core::Property* prop = at->getProperty();
    if (!prop) {
        throw InvalidMoveException("property exception.");
    }
    
    if (prop->getType() != core::PropertyType::STREET) {
        throw InvalidMoveException("no house exception.");
    }

    core::Street* street = static_cast<core::Street*>(prop);
    
    if (street->getHouseCount() == 0 && street->getHotelCount() == 0) {
        throw InvalidMoveException("no building exception");
    }

    if (street->getOwner() != seller) {
        throw InvalidMoveException("non owner exception.");
    }

    int refund = street->getHouseCost() / 2; 
    
    street->demolish(1);
    bank_.receiveHouse(1);
    bank_.pay(*seller, refund);
}
void Game::mortgageProperty(core::Property* prop) {
    core::Player* p = prop->getOwner();
    
    if (p != getCurrentPlayer()) {
        throw InvalidMoveException("mortgage exception");
    }
    if (prop->isMortgagedStatus()) {
        throw InvalidMoveException("mortgage exception.");
    }

    prop->mortgage();
    bank_.pay(*p, prop->getMortgageValue());
}

void Game::unmortgageProperty(core::Property* prop) {
    core::Player* p = prop->getOwner();
    
    if (p != getCurrentPlayer()) {
        throw InvalidMoveException("unmortgage exception");
    }
    if (!prop->isMortgagedStatus()) {
        throw InvalidMoveException("unmortgage exception");
    }

    int cost = prop->getMortgageValue() + (prop->getMortgageValue() * 0.10);
    
    if (!p->canAfford(cost)) {
        throw InsufficientFundsException(p->getBalance(), cost);
    }

    *p -= cost;
    bank_.receive(cost);
    prop->unmortgage();
}

void Game::startAuction(core::Property* prop) {
    if (prop->getOwner() != nullptr) {
        throw InvalidMoveException("auction exception");
    }

    state_ = GameState::AUCTION;
    
    std::vector<core::Player*> eligiblePlayers;
    for (auto* p : players_) {
        if (!p->getIsBankrupt()) {
            eligiblePlayers.push_back(p);
        }
    }
}

void Game::logEvent(const std::string& /*action*/, core::Player& /*p*/, int /*value*/) {
    if (logger_) {
    }
}

void Game::logEvent(const std::string& action, core::Player& p, core::Property& prop, int value) {
    if (logger_) {
    }
}

core::Player* Game::getCurrentPlayer() const {
    if (players_.empty()) throw InvalidMoveException("log exception");
    return players_[currentPlayerId_];
}

GameState Game::getState() const {
    return state_;
}

} /// namespace logic
