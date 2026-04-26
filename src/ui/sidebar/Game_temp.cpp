#include "ui/sidebar/Game_temp.hpp"

namespace temp {

Player::Player(std::string name, std::string avatarPath, long long balance,
               sf::Color currentTileColor, std::string currentTileCode,
               int turnNumber, bool isPlayerTurn,
               std::vector<PropertyTuple> ownedProperties)
    : name_(std::move(name)),
      avatarPath_(std::move(avatarPath)),
      balance_(balance),
      currentTileColor_(currentTileColor),
      currentTileCode_(std::move(currentTileCode)),
      turnNumber_(turnNumber),
      isPlayerTurn_(isPlayerTurn),
      ownedProperties_(std::move(ownedProperties)) {}

Game::Game(int i) : ini_cuma_stub_aja_nanti_ganti_sendiri_(i) {
  players_.emplace_back(
      "COPILOT", "assets/players/copilot.png", 156400,
      sf::Color(sf::Uint32(0xF2F2F2FFU)), "JKT", 21, true,
      std::vector<PropertyTuple>{
          {sf::Color(sf::Uint32(0x955235FFU)), "BDG", 2, false, false, ""},
          {sf::Color(sf::Uint32(0xAAE0FAFFU)), "SBY", 0, false, false, ""},
          {sf::Color(sf::Uint32(0xD93A96FFU)), "SMG", 1, false, false, ""},
          {sf::Color(sf::Uint32(0xFF8B00FFU)), "MLG", 0, false, false, ""},
          {sf::Color(sf::Uint32(0xF11C26FFU)), "DPS", 3, false, false, ""},
          {sf::Color(sf::Uint32(0xFEF200FFU)), "BKS", 0, false, false, ""},
          {sf::Color(sf::Uint32(0x1FB25AFFU)), "MDN", 0, false, false, ""},
          {sf::Color(sf::Uint32(0x0072BBFFU)), "YGY", 5, false, false, ""},
          {sf::Color(sf::Uint32(0x050505FFU)), "KAI", 0, true, false,
           "assets/icons/train.png"},
          {sf::Color(sf::Uint32(0x7F7F7FFFU)), "PLN", 0, false, true,
           "assets/icons/electricity.png"},
          {sf::Color(sf::Uint32(0x4B36D3FFU)), "MKS", 0, false, false, ""},
      });

  players_.emplace_back(
      "CLAUDE", "assets/players/claude.png", 98200,
      sf::Color(sf::Uint32(0x050505FFU)), "BLI", 21, false,
      std::vector<PropertyTuple>{
          {sf::Color(sf::Uint32(0xFF8B00FFU)), "PKU", 0, false, false, ""},
          {sf::Color(sf::Uint32(0xFEF200FFU)), "JBI", 2, false, false, ""},
          {sf::Color(sf::Uint32(0x050505FFU)), "KCI", 0, true, false,
           "assets/icons/train.png"},
      });

  players_.emplace_back(
      "OPENAI", "assets/players/openai.png", 123000,
      sf::Color(sf::Uint32(0x1FB25AFFU)), "BPN", 21, false,
      std::vector<PropertyTuple>{
          {sf::Color(sf::Uint32(0xAAE0FAFFU)), "TGR", 1, false, false, ""},
          {sf::Color(sf::Uint32(0xD93A96FFU)), "BLI", 0, false, false, ""},
          {sf::Color(sf::Uint32(0x7F7F7FFFU)), "PAM", 0, false, true,
           "assets/icons/water.png"},
          {sf::Color(sf::Uint32(0xF11C26FFU)), "BGR", 4, false, false, ""},
      });

  players_.emplace_back(
      "GEMINI", "assets/players/gemini.png", 87500,
      sf::Color(sf::Uint32(0x0072BBFFU)), "JOG", 21, false,
      std::vector<PropertyTuple>{
          {sf::Color(sf::Uint32(0x955235FFU)), "TGL", 0, false, false, ""},
          {sf::Color(sf::Uint32(0x050505FFU)), "MRT", 0, true, false,
           "assets/icons/train.png"},
      });
}

std::vector<Player*> Game::playerPointers() {
  std::vector<Player*> pointers;
  pointers.reserve(players_.size());
  for (Player& player : players_) {
    pointers.push_back(&player);
  }
  return pointers;
}

}  // namespace temp