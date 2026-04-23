#include <iostream>
#include <string>

#include "ui/GameWindow.hpp"

namespace {

constexpr int minTile = 4;
constexpr int maxTile = 14;

bool debugFlag(int argc, char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "--debug") {
      return true;
    }
  }
  return false;
}

}  // namespace

int main(int argc, char* argv[]) {
  if (!debugFlag(argc, argv)) {
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