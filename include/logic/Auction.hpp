#pragma once
#include <vector>

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

  void resolveWinner();
  bool isFinished() const;

  int getCurrentBid() const;
  core::Player* getCurrentWinner() const;
};

}  // namespace logic
