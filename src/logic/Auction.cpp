#include "logic/Auction.hpp"

#include <algorithm>

#include "core/GameException.hpp"
#include "core/Property.hpp"
#include "core/player/Player.hpp"
#include "logic/Bank.hpp"

namespace logic {

namespace {

std::vector<core::Player*> collectEligibleParticipants(
    const std::vector<core::Player*>& players) {
  std::vector<core::Player*> eligiblePlayers;
  eligiblePlayers.reserve(players.size());
  for (core::Player* player : players) {
    if (player != nullptr && !player->isBankrupted()) {
      eligiblePlayers.push_back(player);
    }
  }
  return eligiblePlayers;
}

bool hasParticipant(const std::vector<core::Player*>& participants,
                    core::Player& player) {
  return std::find(participants.begin(), participants.end(), &player) !=
         participants.end();
}

}  // namespace

Auction::Auction(core::Property* subject,
                 const std::vector<core::Player*>& eligiblePlayers)
    : subject_(subject),
      participants_(collectEligibleParticipants(eligiblePlayers)),
      currentBid_(0),
      currentWinner_(nullptr) {}

void Auction::startBid(int startingPrice) {
  if (startingPrice < 0) {
    throw InvalidMoveException("Starting bid cannot be negative.");
  }
  currentBid_ = startingPrice;
}

bool Auction::placeBid(core::Player& p, int amount) {
  if (!hasParticipant(participants_, p)) {
    throw InvalidMoveException("auction participant exception");
  }
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

int Auction::resolveWinner(Bank& bank) {
  if (currentWinner_ == nullptr || subject_ == nullptr) {
    return 0;
  }
  return bank.awardAuction(*currentWinner_, *subject_, currentBid_);
}

bool Auction::isFinished() const { return participants_.size() <= 1; }

int Auction::getCurrentBid() const { return currentBid_; }

core::Player* Auction::getCurrentWinner() const { return currentWinner_; }

const std::vector<core::Player*>& Auction::getParticipants() const {
  return participants_;
}

}  // namespace logic
