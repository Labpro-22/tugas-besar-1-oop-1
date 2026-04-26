#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>

#include "ui/DialogManager.hpp"
#include "ui/board/BoardPanel.hpp"
#include "ui/component/Constants.hpp"
#include "ui/sidebar/ActionPanel.hpp"
#include "ui/sidebar/PlayerInfoPanel.hpp"

namespace ui {
class GameWindow : public sf::RenderWindow {
 public:
  explicit GameWindow(int boardTileCount = 9);
  ~GameWindow();

  void onResize() override;
  void run();
  void close();

 private:
  // Size
  const sf::Vector2f virtualSize_{size::width, size::height};
  const sf::Vector2f mainBoardSize_{size::height, size::height};
  const sf::Vector2f sideBarSize_{size::sideWidth, size::height};

  // SFML View
  sf::Event event_;
  sf::View mainView_;

  // Components
  temp::Game game_;
  BoardPanel boardPanel_;           /**< Main (center) panel */
  PlayerInfoPanel playerInfoPanel_; /**< Right Panel */
  ActionPanel actionPanel_;         /**< Left Panel */
  std::unique_ptr<DialogManager> dialogManager_;
};

}  // namespace ui