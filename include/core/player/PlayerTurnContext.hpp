#pragma once

namespace core {

class Player;
class Property;

class PlayerTurnContext {
 public:
  virtual ~PlayerTurnContext() = default;

  virtual void rollDice() = 0;
  virtual void moveCurrentPlayer() = 0;
  virtual void mortgageProperty(Player& actor, Property& property) = 0;
  virtual void buildHouse(Player& buyer, Property& property) = 0;
};

}  // namespace core