#include "ui/GameWindow.hpp"

#include <SFML/Graphics/View.hpp>

#include "ui/board/BoardPanel.hpp"
#include "ui/component/Constants.hpp"
#include "ui/sidebar/ActionPanel.hpp"
#include "ui/sidebar/PlayerInfoPanel.hpp"

namespace ui {
GameWindow::GameWindow(int boardTileCount)
    : sf::RenderWindow(sf::VideoMode(size::width, size::height), "Nimonspoli"),
      game_(0),
      boardPanel_(boardTileCount),
      playerInfoPanel_(),
      actionPanel_() {
  boardPanel_.setOnTileSelected(
      [this](const TileInfo& info) { actionPanel_.setSelectedTile(info); });
  playerInfoPanel_.setPlayers(game_.playerPointers());
}

// =======
// GameWindow::GameWindow()
//     : sf::RenderWindow(sf::VideoMode(400, 400), "Game Window"),
//       game_(std::vector<core::Player*>{}, nullptr) {
// >>>>>>> main
//   dialogManager_ = std::make_unique<DialogManager>();
// }

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

      // Pass event handler
      boardPanel_.handleEvent(event_, *this);
      playerInfoPanel_.handleEvent(event_, *this);
      actionPanel_.handleEvent(event_, *this);
    }

    // Pass updater
    boardPanel_.update(*this);
    playerInfoPanel_.update(*this);
    actionPanel_.update(*this);
    clear();

    // Pass renderer
    boardPanel_.render(*this);
    playerInfoPanel_.render(*this);
    actionPanel_.render(*this);
    display();
  }
}

void GameWindow::close() {
  // Handle cleanup
  sf::RenderWindow::close();
}

}  // namespace ui