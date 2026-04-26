#pragma once
#include <string>
#include <utility>
#include <vector>

namespace core {
class Player;
class Property;
enum class TileType : int;

enum class TaxType { PPH, PBM };

/**
 * @brief Abstract collaborator handed to tiles and action cards so they can
 *        affect the game without depending on `logic::Game` directly.
 *
 * @note Several methods below are extensions beyond the M1 spec, they are
 *       required to keep cards and tiles decoupled from the concrete game
 *       implementation while still expressing every effect described in the
 *       specification.
 */
class GameContext {
 public:
  virtual ~GameContext() = default;

  // Property
  virtual void offerProperty(Player& p, Property& prop) = 0;
  virtual void chargeRent(Player& p, Property& prop) = 0;

  // Jail
  virtual void sendToJail(Player& p) = 0;

  // Tax
  virtual void chargeTax(Player& p, int flatRate, int percentageRate,
                         TaxType type) = 0;

  // Festival
  virtual void activateFestival(Player& p) = 0;

  // Cards
  virtual void drawChanceCard(Player& p) = 0;
  virtual void drawCommunityChestCard(Player& p) = 0;

  // Bank payments
  virtual void payPlayerFromBank(Player& p, int amount) = 0;

  // Logging
  virtual void logEvent(const std::string& action, Player& p, int value) = 0;
  virtual void logEvent(const std::string& action, Player& p, Property& prop,
                        int value) = 0;

  // Config getters tiles need
  virtual int getGoSalary() const = 0;
  virtual std::pair<int, int> getLastDiceRoll() const = 0;

  /**
   * @brief Returns the live participant list.
   *
   * @note Extension. needed by `LassoCard` / `DemolitionCard` to target
   *       other participants without reaching into `logic::Game` directly.
   */
  virtual const std::vector<Player*>& getPlayers() const = 0;

  /**
   * @brief Returns the number of tiles on the board.
   *
   * @note Extension. needed by movement cards to wrap around the board
   *       without coupling cards to `logic::Board`.
   */
  virtual int getBoardSize() const = 0;

  /**
   * @brief Moves @p p forward to @p targetIndex along the board, firing
   *        `onPassed` for every tile the player crosses (including GO when
   *        wrapping) and finally `onLanded` on the destination.
   *
   * @note Extension. used by `MoveCard`, `ChanceCard::makeMoveBack`,
   *       `ChanceCard::makeAdvanceTo`, and
   *       `ChanceCard::makeAdvanceToNearestRailroad`. Mirrors the normal
   *       dice-roll movement path without re-implementing it inside cards.
   */
  virtual void movePlayer(Player& p, int targetIndex) = 0;

  /**
   * @brief Instantly relocates @p p to @p targetIndex. `onPassed` is NOT
   *        triggered for any intermediate tile, but `onLanded` still fires
   *        on the destination.
   *
   * @note Extension. used by `TeleportCard` and `LassoCard` so that
   *       teleport-style movement does not award GO salary mid-flight. If
   *       the destination itself is GO, `GoTile::onLanded` still pays.
   */
  virtual void teleportPlayer(Player& p, int targetIndex) = 0;

  /**
   * @brief Returns the index of the next clockwise tile of @p type starting
   *        AFTER @p from, or -1 if none exists.
   *
   * @note Extension. used by
   *       `ChanceCard::makeAdvanceToNearestRailroad` (and any future
   *       "advance to nearest X" cards) to locate destinations without
   *       cards iterating over `logic::Board` directly.
   */
  virtual int findNearestTileOfType(int from, TileType type) const = 0;
};

}  // namespace core