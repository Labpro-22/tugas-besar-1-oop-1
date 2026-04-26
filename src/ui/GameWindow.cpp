#include "ui/GameWindow.hpp"

#include <SFML/Graphics/View.hpp>
#include <memory>
#include <vector>

#include "core/player/COMPlayer.hpp"
#include "core/player/GreedyCOMStrategy.hpp"
#include "core/player/HumanPlayer.hpp"
#include "core/player/SimpleCOMStrategy.hpp"
#include "ui/board/BoardPanel.hpp"
#include "ui/component/Constants.hpp"
#include "ui/sidebar/ActionPanel.hpp"
#include "ui/sidebar/PlayerInfoPanel.hpp"

namespace {

std::vector<std::unique_ptr<core::Player>> createDefaultPlayers() {
  std::vector<std::unique_ptr<core::Player>> players;
  players.emplace_back(
      std::make_unique<core::HumanPlayer>("COPILOT", core::Avatar::COPILOT));
  players.emplace_back(std::make_unique<core::COMPlayer>(
      "CLAUDE", core::Avatar::CLAUDE,
      std::make_unique<core::SimpleCOMStrategy>()));
  players.emplace_back(std::make_unique<core::COMPlayer>(
      "CHATGPT", core::Avatar::CHATGPT,
      std::make_unique<core::GreedyCOMStrategy>()));
  players.emplace_back(std::make_unique<core::COMPlayer>(
      "GEMINI", core::Avatar::GEMINI,
      std::make_unique<core::SimpleCOMStrategy>()));
  return players;
}

std::vector<core::Player*> makePlayerPointers(
    std::vector<std::unique_ptr<core::Player>>& players) {
  std::vector<core::Player*> pointers;
  pointers.reserve(players.size());
  for (auto& player : players) {
    pointers.push_back(player.get());
  }
  return pointers;
}

int boardSizeFromSideTileCount(int sideTileCount) {
  return (sideTileCount + 1) * 4;
}

}  // namespace

namespace ui {

GameWindow::GameWindow(int boardTileCount)
    : sf::RenderWindow(sf::VideoMode(size::width, size::height), "Nimonspoli"),
      playerStorage_(createDefaultPlayers()),
      playerPtrs_(makePlayerPointers(playerStorage_)),
      game_(playerPtrs_),
      boardPanel_(boardTileCount),
      playerInfoPanel_(),
      actionPanel_(),
      dialogManager_(*this) {
  game_.setMediator(&dialogManager_);
  game_.initialize(boardSizeFromSideTileCount(boardTileCount), "config");
  game_.startGame();

  actionPanel_.setGameContext(&game_, &dialogManager_);
  boardPanel_.populateTiles(game_.getBoard());

  boardPanel_.setOnTileSelected(
      [this](const TileInfo& info) { actionPanel_.setSelectedTile(info); });
  playerInfoPanel_.setPlayers(game_.getPlayers(), game_.getBoard(),
                              game_.getCurrentPlayer());
}

GameWindow::~GameWindow() = default;

void GameWindow::onResize() {
  mainView_.setSize(virtualSize_);
  mainView_.setCenter(virtualSize_.x / 2.0f, virtualSize_.y / 2.0f);
  float windowRatio =
      static_cast<float>(getSize().x) / static_cast<float>(getSize().y);
  float viewRatio = virtualSize_.x / virtualSize_.y;
  float sizeX = 1.0f;
  float sizeY = 1.0f;
  float posX = 0.0f;
  float posY = 0.0f;

  if (windowRatio > viewRatio) {
    sizeX = viewRatio / windowRatio;
    posX = (1.0f - sizeX) / 2.0f;
  } else {
    sizeY = windowRatio / viewRatio;
    posY = (1.0f - sizeY) / 2.0f;
  }

  mainView_.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
  setView(mainView_);
}

void GameWindow::run() {
  while (isOpen()) {
    while (pollEvent(event_)) {
      if (event_.type == sf::Event::Closed) {
        close();
      }

      boardPanel_.handleEvent(event_, *this);
      playerInfoPanel_.handleEvent(event_, *this);
      actionPanel_.handleEvent(event_, *this);
    }

    playerInfoPanel_.setPlayers(game_.getPlayers(), game_.getBoard(),
                                game_.getCurrentPlayer());

    boardPanel_.update(*this);
    playerInfoPanel_.update(*this);
    actionPanel_.update(*this);
    clear();

    boardPanel_.render(*this);
    playerInfoPanel_.render(*this);
    actionPanel_.render(*this);
    display();
  }
}

void GameWindow::close() { sf::RenderWindow::close(); }

}  // namespace ui
