#include "ui/GameWindow.hpp"

#include <SFML/Graphics/View.hpp>
#include <algorithm>
#include <filesystem>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "core/Tiles.hpp"
#include "core/player/COMPlayer.hpp"
#include "core/player/GreedyCOMStrategy.hpp"
#include "core/player/HumanPlayer.hpp"
#include "core/player/SimpleCOMStrategy.hpp"
#include "data/GameStateDTO.hpp"
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

std::vector<core::Avatar> allAvatars() {
  return {core::Avatar::COPILOT, core::Avatar::CLAUDE, core::Avatar::CHATGPT,
          core::Avatar::GEMINI};
}

std::string avatarName(core::Avatar avatar) {
  switch (avatar) {
    case core::Avatar::COPILOT:
      return "COPILOT";
    case core::Avatar::CLAUDE:
      return "CLAUDE";
    case core::Avatar::CHATGPT:
      return "CHATGPT";
    case core::Avatar::GEMINI:
      return "GEMINI";
    default:
      return "PLAYER";
  }
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

int sideTileCountFromBoardSize(int boardSize) {
  return std::max(4, std::min(14, boardSize / 4 - 1));
}

std::vector<int> discoverBoardSizes() {
  std::vector<int> sizes;
  const std::filesystem::path base("config");
  if (!std::filesystem::exists(base)) {
    return {40};
  }

  for (const auto& entry : std::filesystem::directory_iterator(base)) {
    if (!entry.is_directory()) {
      continue;
    }
    const std::string name = entry.path().filename().string();
    const std::string prefix = "board_";
    if (name.rfind(prefix, 0) != 0) {
      continue;
    }
    try {
      const int boardSize = std::stoi(name.substr(prefix.size()));
      if (boardSize >= 20 && boardSize <= 60 && boardSize % 4 == 0) {
        sizes.push_back(boardSize);
      }
    } catch (...) {
    }
  }

  if (sizes.empty()) {
    sizes.push_back(40);
  }
  std::sort(sizes.begin(), sizes.end());
  sizes.erase(std::unique(sizes.begin(), sizes.end()), sizes.end());
  return sizes;
}

std::vector<std::string> discoverSaveFiles() {
  std::vector<std::string> files;
  const std::filesystem::path base("data");
  if (std::filesystem::exists(base)) {
    for (const auto& entry : std::filesystem::directory_iterator(base)) {
      if (!entry.is_regular_file()) {
        continue;
      }
      if (entry.path().extension() == ".txt") {
        files.push_back(entry.path().string());
      }
    }
  }

  if (files.empty()) {
    files.push_back("data/savegame.txt");
    files.push_back("data/test_save.txt");
  }

  std::sort(files.begin(), files.end());
  files.erase(std::unique(files.begin(), files.end()), files.end());
  return files;
}

}  // namespace

namespace ui {

GameWindow::GameWindow(int boardTileCount)
    : sf::RenderWindow(sf::VideoMode(size::width, size::height), "Nimonspoli"),
      playerStorage_(),
      playerPtrs_(),
      game_(nullptr),
      boardPanel_(boardTileCount),
      playerInfoPanel_(),
      actionPanel_(),
      dialogManager_(*this) {}

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

void GameWindow::rebuildUiBindings() {
  if (!game_) {
    return;
  }

  actionPanel_.setGameContext(game_.get(), &dialogManager_);
  boardPanel_.populateTiles(game_->getBoard());
  boardPanel_.setPlayers(game_->getPlayers());

  boardPanel_.setOnTileSelected([this](const TileInfo& info) {
    if (!game_) {
      return;
    }

    actionPanel_.setSelectedTile(info);
    if (!info.isProperty || info.tileIndex < 0) {
      return;
    }

    core::Tile* tile = game_->getBoard().getTile(info.tileIndex);
    auto* propertyTile = dynamic_cast<core::PropertyTile*>(tile);
    if (propertyTile == nullptr) {
      return;
    }
    dialogManager_.showDeed(propertyTile->getProperty());
  });

  playerInfoPanel_.setOnPlayerSelected([this](core::Player& player) {
    dialogManager_.showOwnedProperties(player);
  });
  playerInfoPanel_.setPlayers(game_->getPlayers(), game_->getBoard(),
                              game_->getCurrentPlayer());
}

bool GameWindow::startNewGame(const StartupConfig& config) {
  if (config.players.size() < 2 || config.players.size() > 4) {
    return false;
  }

  std::vector<std::unique_ptr<core::Player>> storage;
  storage.reserve(config.players.size());
  for (size_t i = 0; i < config.players.size(); ++i) {
    const PlayerSetup& setup = config.players[i];
    if (setup.isComputer) {
      if (i % 2 == 0) {
        storage.emplace_back(std::make_unique<core::COMPlayer>(
            setup.name, setup.avatar, std::make_unique<core::SimpleCOMStrategy>()));
      } else {
        storage.emplace_back(std::make_unique<core::COMPlayer>(
            setup.name, setup.avatar, std::make_unique<core::GreedyCOMStrategy>()));
      }
    } else {
      storage.emplace_back(
          std::make_unique<core::HumanPlayer>(setup.name, setup.avatar));
    }
  }

  playerStorage_ = std::move(storage);
  playerPtrs_ = makePlayerPointers(playerStorage_);
  game_ = std::make_unique<logic::Game>(playerPtrs_);

  game_->setMediator(&dialogManager_);
  game_->initialize(config.boardSize, "config");
  game_->startGame();

  boardPanel_.setTileCount(sideTileCountFromBoardSize(config.boardSize));
  rebuildUiBindings();
  return true;
}

bool GameWindow::loadFromSaveFile(const std::string& path) {
  try {
    data::GameStateDTO dto = data::SaveLoadManager::get().load(path);

    std::vector<std::unique_ptr<core::Player>> storage;
    const std::vector<core::Avatar> avatars = allAvatars();
    for (size_t i = 0; i < dto.players.size(); ++i) {
      const data::PlayerDTO& p = dto.players[i];
      const core::Avatar avatar = avatars[i % avatars.size()];
      if (p.isComputer) {
        storage.emplace_back(std::make_unique<core::COMPlayer>(
            p.name, avatar, std::make_unique<core::SimpleCOMStrategy>()));
      } else {
        storage.emplace_back(std::make_unique<core::HumanPlayer>(p.name, avatar));
      }
    }

    playerStorage_ = std::move(storage);
    playerPtrs_ = makePlayerPointers(playerStorage_);
    game_ = std::make_unique<logic::Game>(playerPtrs_);

    game_->setMediator(&dialogManager_);
    logic::GameLoader::applyDTO(dto, *game_, "config");

    boardPanel_.setTileCount(sideTileCountFromBoardSize(dto.boardSize));
    rebuildUiBindings();
    return true;
  } catch (...) {
    return false;
  }
}

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

bool GameWindow::runNewGameSetup(StartupConfig* outConfig) {
  if (outConfig == nullptr) {
    return false;
  }

  sf::Font& fontRef = AssetsManager::get().getFont(font::primaryFamily);

  // Step 1: choose player count.
  int playerCount = 4;
  {
    bool done = false;
    Panel root({0, 0}, {size::width, size::height}, accent::lightBlue);

    const LabelStyle titleStyle(
        typography::title, palette::black, typography::titleStyle,
        HorizontalAlign::Center, VerticalAlign::Middle, {0, 0}, false,
        typography::logBody);
    root.addChild("new-title", std::make_unique<Label>(
                                    "New Game - Jumlah Pemain", fontRef,
                                    sf::Vector2f(0, size::height * 0.20f),
                                    sf::Vector2f(size::width, 0.6f * size::multiplier),
                                    titleStyle));

    const LabelStyle bLabel(
        typography::buttonPrimary, palette::white, typography::buttonStyle,
        HorizontalAlign::Center, VerticalAlign::Middle, {0, 0}, false,
        typography::logBody);
    const ButtonStyle bStyle(
        accent::darkBlue, accent::blue50, accent::darkBlue,
        component::button::disabled, palette::white, size::lineThickness,
        bLabel);

    const float bw = 0.9f * size::multiplier;
    const float bh = 0.55f * size::multiplier;
    const float by = size::height * 0.42f;
    const float gx = 0.2f * size::multiplier;
    const float total = 3.0f * bw + 2.0f * gx;
    float bx = (size::width - total) * 0.5f;

    auto addCountButton = [&](int count, float x) {
      root.addChild("count-" + std::to_string(count), std::make_unique<Button>(
                                                  std::to_string(count), fontRef,
                                                  sf::Vector2f(x, by),
                                                  sf::Vector2f(bw, bh), bStyle,
                                                  [count, &playerCount, &done]() {
                                                    playerCount = count;
                                                    done = true;
                                                  }));
    };

    addCountButton(2, bx);
    addCountButton(3, bx + bw + gx);
    addCountButton(4, bx + 2.0f * (bw + gx));

    while (isOpen() && !done) {
      while (pollEvent(event_)) {
        if (event_.type == sf::Event::Closed) {
          return false;
        }
        root.handleEvent(event_, *this);
      }
      root.update(*this);
      clear(palette::white);
      root.render(*this);
      display();
    }
  }

  // Step 2: choose avatars and human/com role.
  std::vector<PlayerSetup> setups;
  {
    setups.resize(static_cast<size_t>(playerCount));
    const std::vector<core::Avatar> avatars = allAvatars();
    std::vector<int> avatarIdx(static_cast<size_t>(playerCount), 0);
    std::vector<bool> isCom(static_cast<size_t>(playerCount), true);
    if (!isCom.empty()) {
      isCom[0] = false;
    }
    for (int i = 0; i < playerCount; ++i) {
      avatarIdx[static_cast<size_t>(i)] = i % static_cast<int>(avatars.size());
    }

    bool done = false;
    Panel root({0, 0}, {size::width, size::height}, accent::lightBlue);

    const LabelStyle titleStyle(
        typography::title, palette::black, typography::titleStyle,
        HorizontalAlign::Center, VerticalAlign::Middle, {0, 0}, false,
        typography::logBody);
    root.addChild("setup-title", std::make_unique<Label>(
                                      "Pilih Avatar dan Tipe Pemain", fontRef,
                                      sf::Vector2f(0, size::height * 0.11f),
                                      sf::Vector2f(size::width, 0.55f * size::multiplier),
                                      titleStyle));

    std::vector<Label*> rowLabels;
    rowLabels.reserve(static_cast<size_t>(playerCount));

    const float rowYStart = size::height * 0.24f;
    const float rowGap = 0.64f * size::multiplier;
    const float left = size::width * 0.22f;

    const LabelStyle rowLabelStyle(
        typography::dialogBody, palette::black, typography::titleStyle,
        HorizontalAlign::Left, VerticalAlign::Middle, {0.1f * size::multiplier, 0},
        false, typography::logBody);
    const LabelStyle btnLabel(
        typography::buttonSecondary, palette::white, typography::buttonStyle,
        HorizontalAlign::Center, VerticalAlign::Middle, {0, 0}, false,
        typography::logBody);
    const ButtonStyle btnBlue(
        accent::darkBlue, accent::blue50, accent::darkBlue,
        component::button::disabled, palette::white, size::lineThickness,
        btnLabel);
    const ButtonStyle btnGreen(
        accent::darkGreen, accent::lightGreen, accent::darkGreen,
        component::button::disabled, palette::white, size::lineThickness,
        btnLabel);

    for (int i = 0; i < playerCount; ++i) {
      const float y = rowYStart + static_cast<float>(i) * rowGap;
      auto rowPanel = std::make_unique<Panel>(
          sf::Vector2f(left, y), sf::Vector2f(size::width * 0.56f, 0.52f * size::multiplier),
          PanelStyle(palette::white, palette::lightGrey, size::lineThickness));

      auto rowText = std::make_unique<Label>(
          "", fontRef, sf::Vector2f(left + 0.01f * size::multiplier, y),
          sf::Vector2f(size::width * 0.30f, 0.52f * size::multiplier), rowLabelStyle);
      Label* rowLabelPtr = rowText.get();
      rowLabels.push_back(rowLabelPtr);
      rowPanel->addChild("text", std::move(rowText));

      const float buttonW = 1.05f * size::multiplier;
      const float buttonH = 0.36f * size::multiplier;
      const float by = y + 0.08f * size::multiplier;
      const float avatarX = left + size::width * 0.33f;
      const float roleX = avatarX + buttonW + 0.10f * size::multiplier;

      rowPanel->addChild("avatar-btn", std::make_unique<Button>(
                                      "Avatar", fontRef, sf::Vector2f(avatarX, by),
                                      sf::Vector2f(buttonW, buttonH), btnBlue,
                                      [i, &avatarIdx, avatars]() {
                                        int& idx = avatarIdx[static_cast<size_t>(i)];
                                        idx = (idx + 1) % static_cast<int>(avatars.size());
                                      }));

      rowPanel->addChild("role-btn", std::make_unique<Button>(
                                    "Human/COM", fontRef, sf::Vector2f(roleX, by),
                                    sf::Vector2f(buttonW, buttonH), btnGreen,
                                    [i, &isCom]() {
                                      if (i == 0) {
                                        return;
                                      }
                                      isCom[static_cast<size_t>(i)] =
                                          !isCom[static_cast<size_t>(i)];
                                    }));

      root.addChild("row-" + std::to_string(i), std::move(rowPanel));
    }

    root.addChild("continue", std::make_unique<Button>(
                                    "Lanjut", fontRef,
                                    sf::Vector2f((size::width - 1.6f * size::multiplier) * 0.5f,
                                                 size::height * 0.84f),
                                    sf::Vector2f(1.6f * size::multiplier, 0.45f * size::multiplier),
                                    ButtonStyle(accent::darkBlue, accent::blue50,
                                                accent::darkBlue, component::button::disabled,
                                                palette::white, size::lineThickness, btnLabel),
                                    [&done]() { done = true; }));

    while (isOpen() && !done) {
      for (int i = 0; i < playerCount; ++i) {
        const core::Avatar avatar = avatars[avatarIdx[static_cast<size_t>(i)]];
        const std::string role = (i == 0 || !isCom[static_cast<size_t>(i)]) ? "HUMAN" : "COM";
        rowLabels[static_cast<size_t>(i)]->setText(
            "P" + std::to_string(i + 1) + "  " + avatarName(avatar) + "  [" + role + "]");
      }

      while (pollEvent(event_)) {
        if (event_.type == sf::Event::Closed) {
          return false;
        }
        root.handleEvent(event_, *this);
      }
      root.update(*this);
      clear(palette::white);
      root.render(*this);
      display();
    }

    setups.clear();
    setups.reserve(static_cast<size_t>(playerCount));
    for (int i = 0; i < playerCount; ++i) {
      const core::Avatar avatar = avatars[avatarIdx[static_cast<size_t>(i)]];
      const std::string baseName = avatarName(avatar);
      PlayerSetup setup;
      setup.name = baseName + "_" + std::to_string(i + 1);
      setup.avatar = avatar;
      setup.isComputer = (i == 0) ? false : isCom[static_cast<size_t>(i)];
      setups.push_back(setup);
    }
  }

  // Step 3: roll dice for turn order.
  {
    bool done = false;
    bool rolled = false;
    std::vector<int> dice(setups.size(), 0);
    std::mt19937 rng(std::random_device{}());

    Panel root({0, 0}, {size::width, size::height}, accent::lightBlue);
    const LabelStyle titleStyle(
        typography::title, palette::black, typography::titleStyle,
        HorizontalAlign::Center, VerticalAlign::Middle, {0, 0}, false,
        typography::logBody);
    root.addChild("roll-title", std::make_unique<Label>(
                                     "Roll Dadu untuk Urutan Giliran", fontRef,
                                     sf::Vector2f(0, size::height * 0.14f),
                                     sf::Vector2f(size::width, 0.55f * size::multiplier),
                                     titleStyle));

    std::vector<Label*> lines;
    const LabelStyle lineStyle(
        typography::dialogBody, palette::black, typography::titleStyle,
        HorizontalAlign::Center, VerticalAlign::Middle, {0, 0}, false,
        typography::logBody);
    for (size_t i = 0; i < setups.size(); ++i) {
      auto line = std::make_unique<Label>(
          "", fontRef,
          sf::Vector2f(0, size::height * 0.28f + static_cast<float>(i) * 0.48f * size::multiplier),
          sf::Vector2f(size::width, 0.4f * size::multiplier), lineStyle);
      lines.push_back(line.get());
      root.addChild("roll-line-" + std::to_string(i), std::move(line));
    }

    const LabelStyle btnLabel(
        typography::buttonPrimary, palette::white, typography::buttonStyle,
        HorizontalAlign::Center, VerticalAlign::Middle, {0, 0}, false,
        typography::logBody);

    root.addChild("roll-btn", std::make_unique<Button>(
                                   "ROLL", fontRef,
                                   sf::Vector2f(size::width * 0.38f, size::height * 0.74f),
                                   sf::Vector2f(1.0f * size::multiplier, 0.45f * size::multiplier),
                                   ButtonStyle(accent::darkBlue, accent::blue50,
                                               accent::darkBlue, component::button::disabled,
                                               palette::white, size::lineThickness, btnLabel),
                                   [&rolled, &dice, &rng, &setups]() {
                                     std::uniform_int_distribution<int> dist(1, 6);
                                     for (size_t i = 0; i < setups.size(); ++i) {
                                       dice[i] = dist(rng);
                                     }
                                     rolled = true;
                                   }));

    auto lanjutBtn = std::make_unique<Button>(
        "LANJUT", fontRef,
        sf::Vector2f(size::width * 0.50f, size::height * 0.74f),
        sf::Vector2f(1.2f * size::multiplier, 0.45f * size::multiplier),
        ButtonStyle(accent::darkGreen, accent::lightGreen, accent::darkGreen,
                    component::button::disabled, palette::white,
                    size::lineThickness, btnLabel),
        [&done, &rolled]() {
          if (rolled) {
            done = true;
          }
        });
    root.addChild("next-btn", std::move(lanjutBtn));

    while (isOpen() && !done) {
      for (size_t i = 0; i < setups.size(); ++i) {
        std::string text = setups[i].name;
        if (rolled) {
          text += "  :  " + std::to_string(dice[i]);
        } else {
          text += "  :  -";
        }
        lines[i]->setText(text);
      }

      while (pollEvent(event_)) {
        if (event_.type == sf::Event::Closed) {
          return false;
        }
        root.handleEvent(event_, *this);
      }
      root.update(*this);
      clear(palette::white);
      root.render(*this);
      display();
    }

    if (rolled) {
      std::vector<size_t> idx(setups.size());
      for (size_t i = 0; i < setups.size(); ++i) {
        idx[i] = i;
      }
      std::stable_sort(idx.begin(), idx.end(), [&dice](size_t a, size_t b) {
        return dice[a] > dice[b];
      });

      std::vector<PlayerSetup> ordered;
      ordered.reserve(setups.size());
      for (size_t id : idx) {
        ordered.push_back(setups[id]);
      }
      setups = std::move(ordered);
    }
  }

  // Step 4: choose board configuration.
  int boardSize = 40;
  {
    const std::vector<int> sizes = discoverBoardSizes();
    boardSize = sizes.front();

    bool done = false;
    Panel root({0, 0}, {size::width, size::height}, accent::lightBlue);

    const LabelStyle titleStyle(
        typography::title, palette::black, typography::titleStyle,
        HorizontalAlign::Center, VerticalAlign::Middle, {0, 0}, false,
        typography::logBody);
    root.addChild("board-title", std::make_unique<Label>(
                                      "Pilih Konfigurasi Board", fontRef,
                                      sf::Vector2f(0, size::height * 0.14f),
                                      sf::Vector2f(size::width, 0.55f * size::multiplier),
                                      titleStyle));

    Label* selectedLabel = nullptr;
    const LabelStyle selectedStyle(
        typography::dialogBody, palette::black, typography::titleStyle,
        HorizontalAlign::Center, VerticalAlign::Middle, {0, 0}, false,
        typography::logBody);
    auto selected = std::make_unique<Label>(
        "", fontRef, sf::Vector2f(0, size::height * 0.24f),
        sf::Vector2f(size::width, 0.4f * size::multiplier), selectedStyle);
    selectedLabel = selected.get();
    root.addChild("selected-board", std::move(selected));

    const LabelStyle btnLabel(
        typography::buttonSecondary, palette::white, typography::buttonStyle,
        HorizontalAlign::Center, VerticalAlign::Middle, {0, 0}, false,
        typography::logBody);
    const ButtonStyle bStyle(
        accent::darkBlue, accent::blue50, accent::darkBlue,
        component::button::disabled, palette::white, size::lineThickness,
        btnLabel);

    const int columns = 4;
    const float bw = 0.85f * size::multiplier;
    const float bh = 0.42f * size::multiplier;
    const float gap = 0.12f * size::multiplier;
    const float startX = (size::width - (columns * bw + (columns - 1) * gap)) * 0.5f;
    const float startY = size::height * 0.34f;

    for (size_t i = 0; i < sizes.size(); ++i) {
      const int row = static_cast<int>(i) / columns;
      const int col = static_cast<int>(i) % columns;
      const float x = startX + col * (bw + gap);
      const float y = startY + row * (bh + gap);
      const int value = sizes[i];

      root.addChild("board-" + std::to_string(value), std::make_unique<Button>(
                                                  std::to_string(value), fontRef,
                                                  sf::Vector2f(x, y),
                                                  sf::Vector2f(bw, bh), bStyle,
                                                  [&boardSize, value]() {
                                                    boardSize = value;
                                                  }));
    }

    root.addChild("board-next", std::make_unique<Button>(
                                     "Lanjut", fontRef,
                                     sf::Vector2f((size::width - 1.6f * size::multiplier) * 0.5f,
                                                  size::height * 0.84f),
                                     sf::Vector2f(1.6f * size::multiplier, 0.45f * size::multiplier),
                                     ButtonStyle(accent::darkGreen, accent::lightGreen,
                                                 accent::darkGreen, component::button::disabled,
                                                 palette::white, size::lineThickness, btnLabel),
                                     [&done]() { done = true; }));

    while (isOpen() && !done) {
      selectedLabel->setText("Board terpilih: " + std::to_string(boardSize));

      while (pollEvent(event_)) {
        if (event_.type == sf::Event::Closed) {
          return false;
        }
        root.handleEvent(event_, *this);
      }
      root.update(*this);
      clear(palette::white);
      root.render(*this);
      display();
    }
  }

  outConfig->boardSize = boardSize;
  outConfig->players = setups;
  return true;
}

bool GameWindow::runLoadSetup(std::string* outPath) {
  if (outPath == nullptr) {
    return false;
  }

  sf::Font& fontRef = AssetsManager::get().getFont(font::primaryFamily);
  std::vector<std::string> files = discoverSaveFiles();

  bool done = false;
  std::string selectedPath = files.front();

  Panel root({0, 0}, {size::width, size::height}, accent::lightBlue);
  const LabelStyle titleStyle(
      typography::title, palette::black, typography::titleStyle,
      HorizontalAlign::Center, VerticalAlign::Middle, {0, 0}, false,
      typography::logBody);
  root.addChild("load-title", std::make_unique<Label>(
                                  "Pilih File Save", fontRef,
                                  sf::Vector2f(0, size::height * 0.14f),
                                  sf::Vector2f(size::width, 0.55f * size::multiplier),
                                  titleStyle));

  Label* selectedLabel = nullptr;
  const LabelStyle selectedStyle(
      typography::dialogBody, palette::black, typography::titleStyle,
      HorizontalAlign::Center, VerticalAlign::Middle, {0, 0}, false,
      typography::logBody);
  auto selected = std::make_unique<Label>(
      "", fontRef, sf::Vector2f(0, size::height * 0.24f),
      sf::Vector2f(size::width, 0.4f * size::multiplier), selectedStyle);
  selectedLabel = selected.get();
  root.addChild("load-selected", std::move(selected));

  const LabelStyle btnLabel(
      typography::buttonSecondary, palette::white, typography::buttonStyle,
      HorizontalAlign::Center, VerticalAlign::Middle, {0, 0}, false,
      typography::logBody);
  const ButtonStyle listStyle(
      accent::darkBlue, accent::blue50, accent::darkBlue,
      component::button::disabled, palette::white, size::lineThickness,
      btnLabel);

  float y = size::height * 0.33f;
  for (size_t i = 0; i < files.size() && i < 8; ++i) {
    const std::string path = files[i];
    root.addChild("save-" + std::to_string(i), std::make_unique<Button>(
                                             path, fontRef,
                                             sf::Vector2f(size::width * 0.24f, y),
                                             sf::Vector2f(size::width * 0.52f, 0.40f * size::multiplier),
                                             listStyle,
                                             [&selectedPath, path]() {
                                               selectedPath = path;
                                             }));
    y += 0.46f * size::multiplier;
  }

  root.addChild("load-next", std::make_unique<Button>(
                                  "Muat", fontRef,
                                  sf::Vector2f((size::width - 1.6f * size::multiplier) * 0.5f,
                                               size::height * 0.86f),
                                  sf::Vector2f(1.6f * size::multiplier, 0.45f * size::multiplier),
                                  ButtonStyle(accent::darkGreen, accent::lightGreen,
                                              accent::darkGreen, component::button::disabled,
                                              palette::white, size::lineThickness, btnLabel),
                                  [&done]() { done = true; }));

  while (isOpen() && !done) {
    selectedLabel->setText("Terpilih: " + selectedPath);

    while (pollEvent(event_)) {
      if (event_.type == sf::Event::Closed) {
        return false;
      }
      root.handleEvent(event_, *this);
    }
    root.update(*this);
    clear(palette::white);
    root.render(*this);
    display();
  }

  *outPath = selectedPath;
  return true;
}

void GameWindow::run() {
  while (isOpen()) {
    const StartupChoice choice = showStartupScreen();
    if (choice == StartupChoice::Exit) {
      close();
      return;
    }

    if (choice == StartupChoice::NewGame) {
      StartupConfig config;
      if (!runNewGameSetup(&config)) {
        continue;
      }
      if (startNewGame(config)) {
        break;
      }
      showStartupError("Gagal Setup", "Konfigurasi New Game tidak valid.");
      continue;
    }

    std::string loadPath;
    if (!runLoadSetup(&loadPath)) {
      continue;
    }
    if (loadFromSaveFile(loadPath)) {
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

    if (!game_) {
      continue;
    }

    playerInfoPanel_.setPlayers(game_->getPlayers(), game_->getBoard(),
                                game_->getCurrentPlayer());
    boardPanel_.setPlayers(game_->getPlayers());

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
