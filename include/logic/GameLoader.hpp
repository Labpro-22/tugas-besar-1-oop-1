#pragma once
#include <string>
#include <vector>

#include "data/GameStateDTO.hpp"
#include "data/LogEntry.hpp"

namespace logic {
class Game;

/**
 * @brief Restores a Game from a saved GameStateDTO.
 *
 * Urutan operasi dalam applyDTO():
 *   1. game.initialize(dto.boardSize, configPath) : board rebuild dari config
 *   2. restoreGameMeta()   : turnCount, maxTurn, turnOrder, active player, log
 *   3. restorePlayers()    : balance, position, jail state, kartu tangan
 *   4. restoreProperties() : ownership, buildings, festival state
 *   5. restoreDeck()       : skill deck order (kartu tangan sudah dihandle di
 * step 3)
 *
 * Precondition: game.players_ sudah terisi dengan Player objects yang namanya
 * cocok dengan dto.players sebelum applyDTO dipanggil.
 */
class GameLoader {
 public:
  static void applyDTO(const data::GameStateDTO& dto, Game& game,
                       const std::string& configPath);

 private:
  static void restoreGameMeta(const data::GameStateDTO& dto, Game& game);
  static void restorePlayers(const data::GameStateDTO& dto, Game& game);
  static void restoreProperties(const data::GameStateDTO& dto, Game& game);
  static void restoreDeck(const data::GameStateDTO& dto, Game& game);
};

}  // namespace logic