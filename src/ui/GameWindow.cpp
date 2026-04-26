#include "ui/GameWindow.hpp"

#include <SFML/Graphics/View.hpp>
#include <memory>
#include <string>
#include <vector>

#include "core/Tiles.hpp"
#include "core/player/COMPlayer.hpp"
#include "core/player/GreedyCOMStrategy.hpp"
#include "core/player/HumanPlayer.hpp"
#include "core/player/SimpleCOMStrategy.hpp"
#include "data/SaveLoadManager.hpp"
#include "logic/GameLoader.hpp"
#include "ui/AssetsManager.hpp"
#include "ui/board/BoardPanel.hpp"
#include "ui/component/Color.hpp"
#include "ui/component/Constants.hpp"
#include "ui/component/Style.hpp"
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
  boardPanel_.setPlayers(game_.getPlayers());

  boardPanel_.setOnTileSelected(
      [this](const TileInfo& info) {
        actionPanel_.setSelectedTile(info);
        if (!info.isProperty || info.tileIndex < 0) {
          return;
        }

        core::Tile* tile = game_.getBoard().getTile(info.tileIndex);
        auto* propertyTile = dynamic_cast<core::PropertyTile*>(tile);
        if (propertyTile == nullptr) {
          return;
        }
        dialogManager_.showDeed(propertyTile->getProperty());
      });
  playerInfoPanel_.setPlayers(game_.getPlayers(), game_.getBoard(),
                              game_.getCurrentPlayer());
  playerInfoPanel_.setOnPlayerSelected(
      [this](core::Player& player) { dialogManager_.showOwnedProperties(player); });
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
    const StartupChoice choice = showStartupScreen();
    if (choice == StartupChoice::Exit) {
      close();
      return;
    }
    if (choice == StartupChoice::NewGame) {
      break;
    }
    if (loadFromSaveFile("data/savegame.txt")) {
      break;
    }
    showStartupError("Gagal Memuat", "File save tidak valid atau tidak ditemukan.");
  }

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
    boardPanel_.setPlayers(game_.getPlayers());

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

GameWindow::StartupChoice GameWindow::showStartupScreen() {
  sf::Font& fontRef = AssetsManager::get().getFont(font::primaryFamily);

  StartupChoice choice = StartupChoice::Exit;
  bool done = false;

  Panel root({0, 0}, {size::width, size::height}, accent::lightBlue);

  const float logoW = 4.6f * size::multiplier;
  const float logoH = 1.8f * size::multiplier;
  auto logo = std::make_unique<ImagePanel>(
      sf::Vector2f((size::width - logoW) * 0.5f, size::height * 0.16f),
      sf::Vector2f(logoW, logoH), "assets/icons/nimonspoli.png");
  root.addChild("startup-logo", std::move(logo));

  const LabelStyle titleStyle(
      typography::title, palette::black, typography::titleStyle,
      HorizontalAlign::Center, VerticalAlign::Middle, {0, 0}, false,
      typography::logBody);
  root.addChild("startup-title", std::make_unique<Label>(
                                   "Selamat Datang", fontRef,
                                   sf::Vector2f(0, size::height * 0.40f),
                                   sf::Vector2f(size::width, 0.6f * size::multiplier),
                                   titleStyle));

  const LabelStyle buttonLabel(
      typography::buttonPrimary, palette::white, typography::buttonStyle,
      HorizontalAlign::Center, VerticalAlign::Middle, {0, 0}, false,
      typography::logBody);

  const float buttonW = 3.2f * size::multiplier;
  const float buttonH = 0.55f * size::multiplier;
  const float buttonX = (size::width - buttonW) * 0.5f;
  const float buttonY = size::height * 0.52f;

  const ButtonStyle newGameStyle(
      accent::darkBlue, accent::blue50, accent::darkBlue,
      component::button::disabled, palette::white, size::lineThickness,
      buttonLabel);
  const ButtonStyle continueStyle(
      accent::darkGreen, accent::lightGreen, accent::darkGreen,
      component::button::disabled, palette::white, size::lineThickness,
      buttonLabel);
  const ButtonStyle exitStyle(
      accent::darkRed, accent::pink, accent::darkRed,
      component::button::disabled, palette::white, size::lineThickness,
      buttonLabel);

  root.addChild("startup-new", std::make_unique<Button>(
                                 "New Game", fontRef,
                                 sf::Vector2f(buttonX, buttonY),
                                 sf::Vector2f(buttonW, buttonH), newGameStyle,
                                 [&done, &choice]() {
                                   choice = StartupChoice::NewGame;
                                   done = true;
                                 }));

  root.addChild("startup-continue", std::make_unique<Button>(
                                      "Continue Existing Game", fontRef,
                                      sf::Vector2f(buttonX, buttonY + buttonH + 0.12f * size::multiplier),
                                      sf::Vector2f(buttonW, buttonH), continueStyle,
                                      [&done, &choice]() {
                                        choice = StartupChoice::ContinueGame;
                                        done = true;
                                      }));

  root.addChild("startup-exit", std::make_unique<Button>(
                                  "Exit", fontRef,
                                  sf::Vector2f(buttonX, buttonY + 2.0f * (buttonH + 0.12f * size::multiplier)),
                                  sf::Vector2f(buttonW, buttonH), exitStyle,
                                  [&done, &choice]() {
                                    choice = StartupChoice::Exit;
                                    done = true;
                                  }));

  while (isOpen() && !done) {
    while (pollEvent(event_)) {
      if (event_.type == sf::Event::Closed) {
        choice = StartupChoice::Exit;
        done = true;
        break;
      }
      root.handleEvent(event_, *this);
    }

    root.update(*this);
    clear(palette::white);
    root.render(*this);
    display();
  }

  return choice;
}

bool GameWindow::loadFromSaveFile(const std::string& path) {
  try {
    data::GameStateDTO dto = data::SaveLoadManager::get().load(path);
    logic::GameLoader::applyDTO(dto, game_, "config");
    boardPanel_.populateTiles(game_.getBoard());
    boardPanel_.setPlayers(game_.getPlayers());
    playerInfoPanel_.setPlayers(game_.getPlayers(), game_.getBoard(),
                                game_.getCurrentPlayer());
    return true;
  } catch (...) {
    return false;
  }
}

void GameWindow::showStartupError(const std::string& title,
                                  const std::string& message) {
  sf::Font& fontRef = AssetsManager::get().getFont(font::primaryFamily);
  bool done = false;

  Panel overlay({0, 0}, {size::width, size::height}, PanelStyle(palette::black20));
  const sf::Vector2f modalSize(4.4f * size::multiplier, 2.6f * size::multiplier);
  const sf::Vector2f modalPos((size::width - modalSize.x) * 0.5f,
                              (size::height - modalSize.y) * 0.5f);

  auto modal = std::make_unique<Panel>(
      modalPos, modalSize,
      PanelStyle(palette::white, palette::lightGrey, size::lineThickness));

  const LabelStyle titleStyle(
      typography::sectionTitle, palette::black, typography::titleStyle,
      HorizontalAlign::Center, VerticalAlign::Middle, {0, 0}, false,
      typography::logBody);
  modal->addChild("error-title", std::make_unique<Label>(
                                    title, fontRef, modalPos,
                                    sf::Vector2f(modalSize.x, 0.7f * size::multiplier),
                                    titleStyle));

  const LabelStyle bodyStyle(
      typography::dialogBody, palette::black, typography::regular,
      HorizontalAlign::Center, VerticalAlign::Middle,
      {0.2f * size::multiplier, 0}, true, typography::logBody);
  modal->addChild("error-body", std::make_unique<Label>(
                                   message, fontRef,
                                   sf::Vector2f(modalPos.x, modalPos.y + 0.75f * size::multiplier),
                                   sf::Vector2f(modalSize.x, 0.95f * size::multiplier),
                                   bodyStyle));

  const LabelStyle okLabel(
      typography::buttonPrimary, palette::white, typography::buttonStyle,
      HorizontalAlign::Center, VerticalAlign::Middle, {0, 0}, false,
      typography::logBody);
  const ButtonStyle okStyle(
      accent::darkBlue, accent::blue50, accent::darkBlue,
      component::button::disabled, palette::white, size::lineThickness,
      okLabel);
  modal->addChild("error-ok", std::make_unique<Button>(
                                 "OK", fontRef,
                                 sf::Vector2f(modalPos.x + (modalSize.x - 1.2f * size::multiplier) * 0.5f,
                                              modalPos.y + modalSize.y - 0.75f * size::multiplier),
                                 sf::Vector2f(1.2f * size::multiplier, 0.45f * size::multiplier),
                                 okStyle, [&done]() { done = true; }));

  overlay.addChild("error-modal", std::move(modal));

  while (isOpen() && !done) {
    while (pollEvent(event_)) {
      if (event_.type == sf::Event::Closed) {
        close();
        return;
      }
      overlay.handleEvent(event_, *this);
    }
    overlay.update(*this);
    clear(palette::white);
    overlay.render(*this);
    display();
  }
}

}  // namespace ui
