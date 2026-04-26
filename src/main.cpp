#include <iostream>
#include <string>

#include "data/ConfigReader.hpp"
#include "data/SaveLoadManager.hpp"
#include "data/TransactionLogger.hpp"
#include "ui/GameWindow.hpp"

namespace {

constexpr int minTile = 4;
constexpr int maxTile = 14;

bool hasFlag(int argc, char* argv[], const std::string& target) {
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == target) {
      return true;
    }
  }
  return false;
}

bool testDataLayer() {
  try {
    std::cout << "[DataTest] Starting...\n";

    data::ConfigReader::initialize("config", 40);
    auto& reader = data::ConfigReader::get();

    auto properties = reader.readProperties();
    auto actions = reader.readActionTiles();
    auto tax = reader.readTax();
    auto special = reader.readSpecial();
    auto misc = reader.readMisc();
    auto railRents = reader.readRailroadRents();
    auto utilityMultipliers = reader.readUtilityMultipliers();
    auto tiles = reader.buildBoard();

    std::cout << "[DataTest] ConfigReader PASS (properties="
              << properties.size() << ", actionTiles=" << actions.size()
              << ", boardTiles=" << tiles.size() << ")\n";

    (void)tax;
    (void)special;
    (void)misc;
    (void)railRents;
    (void)utilityMultipliers;

    auto& logger = data::TransactionLogger::get();
    logger.clear();
    logger.log(1, "P1", data::LogAction::DICE_ROLL, "Rolled 7");
    logger.log(1, "P1", data::LogAction::PIECE_MOVEMENT, "Moved to GO");
    logger.log(2, "P2", data::LogAction::TAX_PAYMENT, "Paid M200");
    logger.serialize("data/test_log.txt");
    logger.clear();
    logger.deserialize("data/test_log.txt");

    if (logger.getEntries().size() != 3) {
      std::cout << "[DataTest] Logger FAIL\n";
      return false;
    }
    std::cout << "[DataTest] Logger PASS\n";

    data::GameStateDTO state;
    state.boardSize = 40;
    state.turnCount = 3;
    state.maxTurn = 200;
    state.currentPlayerIdx = 0;
    state.turnOrder = {"P1", "P2"};
    state.activePlayerName = "P1";

    data::PlayerDTO p1;
    p1.name = "P1";
    p1.balance = 1500;
    p1.positionCode = "GO";
    p1.isComputer = false;

    data::PlayerDTO p2;
    p2.name = "P2";
    p2.balance = 1500;
    p2.positionCode = "GO";
    p2.isComputer = true;

    state.players.push_back(p1);
    state.players.push_back(p2);

    data::SaveLoadManager::get().save(state, "data/test_save.txt");
    data::GameStateDTO loaded =
        data::SaveLoadManager::get().load("data/test_save.txt");

    if (loaded.boardSize != 40 || loaded.turnCount != 3 ||
        loaded.players.size() != 2) {
      std::cout << "[DataTest] SaveLoad FAIL\n";
      return false;
    }

    std::cout << "[DataTest] SaveLoad PASS\n";
    std::cout << "[DataTest] Completed successfully.\n";
    return true;
  } catch (const std::exception& e) {
    std::cout << "[DataTest] ERROR: " << e.what() << "\n";
    return false;
  }
}

}  // namespace

int main(int argc, char* argv[]) {
  if (hasFlag(argc, argv, "--test-data")) {
    return testDataLayer() ? 0 : 1;
  }

  if (!hasFlag(argc, argv, "--debug")) {
    ui::GameWindow gameWindow;
    gameWindow.run();
    return 0;
  }

  std::cout << "DEBUG BOARD"
            << " Enter tile count per side in range [" << minTile << ", "
            << maxTile << "].\n";

  while (true) {
    std::cout << "> " << std::flush;

    int tileCount = 0;
    if (!(std::cin >> tileCount)) {
      if (std::cin.eof()) {
        break;
      }

      std::cin.clear();
      std::string discarded;
      std::getline(std::cin, discarded);
      std::cout << "Invalid input. Please enter a valid integer.\n";
      continue;
    }

    if (tileCount < minTile || tileCount > maxTile) {
      std::cout << "Value out of range. Allowed: [" << minTile << ", "
                << maxTile << "].\n";
      continue;
    }

    ui::GameWindow previewWindow(tileCount);
    previewWindow.run();

    std::cout
        << "Preview window closed. Enter another value or Ctrl+C to exit.\n";
  }

  return 0;
}