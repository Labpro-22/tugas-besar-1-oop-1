#include "logic/Game.hpp"
#include "core/Player.hpp"
#include "core/Property.hpp"
#include "core/Tiles.hpp"
#include "core/GameException.hpp"
#include "logic/Auction.hpp"

#include <cstdlib>
#include <ctime>
#include <algorithm>

namespace logic {

Game::Game(std::vector<core::Player*> players, TransactionLogger* logger)
    : players_(players), logger_(logger), currentPlayerId_(0), 
      state_(GameState::PRE_ROLL), lastDiceRoll_({0, 0}), turnCount_(1) {
    // random dice roll 
	      std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void Game::startGame() {
    state_ = GameState::PRE_ROLL;
    // open for modification,  
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
    } while (players_[currentPlayerId_]->getIsBankrupt()); // Skip bankrupt players

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
        throw InvalidMoveException("Tidak bisa melempar dadu pada state ini.");
    }

    lastDiceRoll_.first = (std::rand() % 6) + 1;
    lastDiceRoll_.second = (std::rand() % 6) + 1;
    
}

void Game::moveCurrentPlayer() {
    core::Player* p = getCurrentPlayer();
    
    int steps = lastDiceRoll_.first + lastDiceRoll_.second;
    int boardSize = board_.getTileCount();
    
    if (boardSize == 0) {
        throw InvalidMoveException("Papan permainan belum dimuat!");
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
    if (!tile) throw InvalidMoveException("Tile tidak valid.");
    
    state_ = GameState::TILE_ACTION;
    core::Player* p = getCurrentPlayer();
    
    tile->onLanded(*p, *this);
}

void Game::buyProperty(core::Property* prop) {
    core::Player* p = getCurrentPlayer();
    int price = prop->getPrice();

    if (prop->getOwner() != nullptr) {
        throw InvalidMoveException("Properti sudah dimiliki oleh pemain lain.");
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
    auto* propTile = dynamic_cast<core::PropertyTile*>(at);
    if (!propTile) throw InvalidMoveException("Bukan petak properti.");
    
    auto* street = dynamic_cast<core::Street*>(propTile->getProperty());
    if (!street) throw InvalidMoveException("Hanya bisa membangun di properti tipe Street.");

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
    street->addHouse();

}

void Game::sellHouse(core::Player* seller, core::Tile* at) {
    auto* propTile = dynamic_cast<core::PropertyTile*>(at);
    if (!propTile) throw InvalidMoveException("Bukan petak properti.");
    
    auto* street = dynamic_cast<core::Street*>(propTile->getProperty());
    if (!street || street->getHouseCount() == 0) {
        throw InvalidMoveException("Tidak ada rumah yang bisa dijual di properti ini.");
    }

    if (street->getOwner() != seller) {
        throw InvalidMoveException("Anda bukan pemilik properti ini.");
    }

    int refund = street->getHouseCost() / 2;
    
    street->removeHouse();
    bank_.receiveHouse(1);
    bank_.pay(*seller, refund);
}

void Game::mortgageProperty(core::Property* prop) {
    core::Player* p = prop->getOwner();
    
    if (p != getCurrentPlayer()) {
        throw InvalidMoveException("Hanya pemilik yang dapat menggadaikan properti.");
    }
    if (prop->getIsMortgaged()) {
        throw InvalidMoveException("Properti ini sudah digadaikan.");
    }

    prop->setMortgaged(true);
    bank_.pay(*p, prop->getMortgageValue());
    
}


void Game::unmortgageProperty(core::Property* prop) {
    core::Player* p = prop->getOwner();
    
    if (p != getCurrentPlayer()) {
        throw InvalidMoveException("Hanya pemilik yang dapat menebus properti.");
    }
    if (!prop->getIsMortgaged()) {
        throw InvalidMoveException("Properti ini tidak sedang digadaikan.");
    }

    int cost = prop->getMortgageValue() + (prop->getMortgageValue() * 0.10);
    
    if (!p->canAfford(cost)) {
        throw InsufficientFundsException(p->getBalance(), cost);
    }

    *p -= cost;
    bank_.receive(cost);
    prop->setMortgaged(false);
    
}

void Game::startAuction(core::Property* prop) {
    if (prop->getOwner() != nullptr) {
        throw InvalidMoveException("Properti sudah dimiliki, tidak bisa dilelang.");
    }

    state_ = GameState::AUCTION;
    
    std::vector<core::Player*> eligiblePlayers;
    for (auto* p : players_) {
        if (!p->getIsBankrupt()) {
            eligiblePlayers.push_back(p);
        }
    }
}

void Game::logEvent(const std::string& action, core::Player& p, int value) {
    if (logger_) {
    }
}

void Game::logEvent(const std::string& action, core::Player& p, core::Property& prop, int value) {
    if (logger_) {
    }
}

core::Player* Game::getCurrentPlayer() const {
    if (players_.empty()) throw InvalidMoveException("Tidak ada pemain dalam permainan.");
    return players_[currentPlayerId_];
}

GameState Game::getState() const {
    return state_;
}

} // namespace logic
