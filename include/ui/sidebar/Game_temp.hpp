#pragma once

#include <SFML/Graphics/Color.hpp>
#include <string>
#include <tuple>
#include <vector>

namespace temp {

using PropertyTuple =
    std::tuple<sf::Color, std::string, int, bool, bool, std::string>;

class Player {
 public:
  Player(std::string name, std::string avatarPath, long long balance,
         sf::Color currentTileColor, std::string currentTileCode,
         int turnNumber, bool isPlayerTurn,
         std::vector<PropertyTuple> ownedProperties);

  const std::string& name() const { return name_; }
  const std::string& avatarPath() const { return avatarPath_; }
  long long balance() const { return balance_; }
  const sf::Color& currentTileColor() const { return currentTileColor_; }
  const std::string& currentTileCode() const { return currentTileCode_; }
  int turnNumber() const { return turnNumber_; }
  bool isPlayerTurn() const { return isPlayerTurn_; }
  const std::vector<PropertyTuple>& ownedProperties() const {
    return ownedProperties_;
  }

 private:
  std::string name_;
  std::string avatarPath_;
  long long balance_;
  sf::Color currentTileColor_;
  std::string currentTileCode_;
  int turnNumber_;
  bool isPlayerTurn_;
  std::vector<PropertyTuple> ownedProperties_;
};

class Game {
 public:
  Game(int i);

  std::vector<const Player*> playerPointers() const;

 private:
  int ini_cuma_stub_aja_nanti_ganti_sendiri_;
  std::vector<Player> players_;
};

}  // namespace temp