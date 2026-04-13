#include "ui/GameWindow.hpp"

namespace ui {
GameWindow::GameWindow()
    : sf::RenderWindow(sf::VideoMode(400, 400), "Game Window"), game_(0) {
  dialogManager_ = std::make_unique<DialogManager>();
}

GameWindow::~GameWindow() = default;

void GameWindow::onCreate() {
  // Initialize panels and dialog manager
}

void GameWindow::onResize() {
  // Handle window resizing
}

void GameWindow::run() {
  while (isOpen()) {
    while (pollEvent(event_)) {
      if (event_.type == sf::Event::Closed) {
        close();
      }
      boardPanel_.handleEvent(event_, *this);
      logPanel_.handleEvent(event_, *this);
      playerInfoPanel_.handleEvent(event_, *this);
      actionPanel_.handleEvent(event_, *this);
    }

    boardPanel_.update(*this);
    logPanel_.update(*this);
    playerInfoPanel_.update(*this);
    actionPanel_.update(*this);

    clear();
    boardPanel_.render(*this);
    logPanel_.render(*this);
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