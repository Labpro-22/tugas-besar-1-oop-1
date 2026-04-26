#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <string>
#include <memory>
#include <vector>

#include "core/player/Player.hpp"
#include "logic/Game.hpp"
#include "ui/board/BoardPanel.hpp"
#include "ui/component/Constants.hpp"
#include "ui/dialog/DialogManager.hpp"
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
  enum class StartupChoice { NewGame, ContinueGame, Exit };

  StartupChoice showStartupScreen();
  bool loadFromSaveFile(const std::string& path);
  void showStartupError(const std::string& title, const std::string& message);

  // Size
  const sf::Vector2f virtualSize_{size::width, size::height};
  const sf::Vector2f mainBoardSize_{size::height, size::height};
  const sf::Vector2f sideBarSize_{size::sideWidth, size::height};

  // SFML View
  sf::Event event_;
  sf::View mainView_;

  // Components
  std::vector<std::unique_ptr<core::Player>> playerStorage_;
  std::vector<core::Player*> playerPtrs_;
  logic::Game game_;
  BoardPanel boardPanel_;           /**< Main (center) panel */
  PlayerInfoPanel playerInfoPanel_; /**< Right Panel */
  ActionPanel actionPanel_;         /**< Left Panel */
  DialogManager dialogManager_;     /**< Modal dialog mediator */
};

}  // namespace ui