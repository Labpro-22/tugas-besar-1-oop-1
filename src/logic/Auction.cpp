#include "logic/Auction.hpp"

#include <algorithm>

#include "core/GameException.hpp"
#include "core/Property.hpp"
#include "core/player/Player.hpp"

namespace logic {

Auction::Auction(core::Property* subject,
                 const std::vector<core::Player*>& eligiblePlayers)
    : subject_(subject),
      participants_(eligiblePlayers),
      currentBid_(0),
      currentWinner_(nullptr) {}

void Auction::startBid(int startingPrice) { currentBid_ = startingPrice; }

bool Auction::placeBid(core::Player& p, int amount) {
  if (amount <= currentBid_) {
    throw InvalidMoveException("Tawaran M" + std::to_string(amount) +
                               " harus lebih tinggi dari M" +
                               std::to_string(currentBid_));
  }
  if (!p.canAfford(amount)) {
    throw InsufficientFundsException(p.getBalance(), amount);
  }

  currentBid_ = amount;
  currentWinner_ = &p;
  return true;
}

void Auction::passBid(core::Player& p) {
  participants_.erase(
      std::remove(participants_.begin(), participants_.end(), &p),
      participants_.end());
}

void Auction::resolveWinner() {
  if (currentWinner_ != nullptr) {
    *currentWinner_ -= currentBid_;
    subject_->setOwner(currentWinner_);
    currentWinner_->addProperty(*subject_);
  }
}

bool Auction::isFinished() const { return participants_.size() <= 1; }

int Auction::getCurrentBid() const { return currentBid_; }

core::Player* Auction::getCurrentWinner() const { return currentWinner_; }

}  // namespace logic
