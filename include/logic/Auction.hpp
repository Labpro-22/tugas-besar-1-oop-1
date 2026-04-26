#pragma once

#include <vector>

#include "logic/Bank.hpp"
#include "logic/UIInputMediator.hpp"

namespace core {
class Player;
class Property;
}  // namespace core

namespace logic {

class Auction {
 private:
  core::Property* subject_;
  std::vector<core::Player*> participants_;
  int currentBid_;
  core::Player* currentWinner_;

 public:
  Auction(core::Property* subject,
          const std::vector<core::Player*>& eligiblePlayers);
  ~Auction() = default;

  void startBid(int startingPrice);
  bool placeBid(core::Player& p, int amount);
  void passBid(core::Player& p);

  int resolveWinner(Bank& bank);
  bool isFinished() const;

  int getCurrentBid() const;
  core::Player* getCurrentWinner() const;
  const std::vector<core::Player*>& getParticipants() const;
};

}  // namespace logic
