#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>

#include "logic/Game.hpp"
#include "ui/Widgets.hpp"

namespace ui {

// Ini masih stub nanti diganti class

struct BoardPanel : public Panel {
  BoardPanel() : Panel({0, 0}, {200, 200}, sf::Color::Green) {}
  ~BoardPanel() override = default;
};

struct LogPanel : public Panel {
  LogPanel() : Panel({200, 0}, {200, 200}, sf::Color::Yellow) {}
  ~LogPanel() override = default;
};

struct PlayerInfoPanel : public Panel {
  PlayerInfoPanel() : Panel({0, 200}, {200, 200}, sf::Color::Cyan) {}
  ~PlayerInfoPanel() override = default;
};

struct ActionPanel : public Panel {
  ActionPanel() : Panel({200, 200}, {200, 200}, sf::Color::Magenta) {}
  ~ActionPanel() override = default;
};

struct DialogManager {};

class GameWindow : public sf::RenderWindow {
public:
  GameWindow();
  ~GameWindow();

  void onCreate() override;
  void onResize() override;
  void run();
  void close();

private:
  sf::Event event_;
  logic::Game game_;
  BoardPanel boardPanel_;
  LogPanel logPanel_;
  PlayerInfoPanel playerInfoPanel_;
  ActionPanel actionPanel_;
  std::unique_ptr<DialogManager> dialogManager_;
};

} // namespace ui