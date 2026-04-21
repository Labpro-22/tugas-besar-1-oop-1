#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>

#include "logic/Game.hpp"
#include "ui/ActionPanel.hpp"
#include "ui/BoardPanel.hpp"
#include "ui/Constants.hpp"
#include "ui/DialogManager.hpp"
#include "ui/PlayerInfoPanel.hpp"

namespace ui {
class GameWindow : public sf::RenderWindow {
 public:
  GameWindow();
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
  logic::Game game_;
  BoardPanel boardPanel_;           /**< Main (center) panel */
  PlayerInfoPanel playerInfoPanel_; /**< Right Panel */
  ActionPanel actionPanel_;         /**< Left Panel */
  std::unique_ptr<DialogManager> dialogManager_;
};

}  // namespace ui