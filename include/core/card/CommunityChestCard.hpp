#pragma once

#include <core/card/Card.hpp>
#include <core/player/Player.hpp>
#include <functional>
#include <memory>
#include <string>

namespace core {

class GameContext;

/**
 * @brief Community Chest deck entry mirroring `ChanceCard` mechanics.
 */
class CommunityChestCard : public ActionCard {
 public:
  /**
   * @brief Build a card with a custom functor effect.
   * @param description Text printed when the card is revealed.
   * @param effect Functor that mutates `player` and `context`.
   * @note Extension (not in spec); the spec only lists `execute` /
   * `getCardType`.
   */
  CommunityChestCard(std::string description,
                     std::function<void(Player&, GameContext&)> effect);

  void execute(Player& player, GameContext& context) override;

  std::string getCardType() const override;

  /**
   * @brief Factory for doctor / tax style bank payments.
   * @param amount Funds removed from the player and credited to the bank.
   * @param description Card flavour text.
   * @note Extension (not in spec).
   */
  static std::unique_ptr<ActionCard> makePayBank(int amount,
                                                 std::string description);

  /**
   * @brief Factory for grants paid from the bank to the player.
   * @param amount Funds added to the player and debited from the bank pool.
   * @param description Card flavour text.
   * @note Extension (not in spec).
   */
  static std::unique_ptr<ActionCard> makeCollect(int amount,
                                                 std::string description);

  /**
   * @brief Factory for the spec card "Ulang tahun". collect @p amount
   *        from each non-bankrupt opponent. Bankrupt players and the
   *        executing player itself are skipped.
   * @param amount Funds transferred from each opponent to the executing player.
   * @param description Card flavour text.
   * @note Spec card from `Spesifikasi.md:478`.
   */
  static std::unique_ptr<ActionCard> makeCollectFromAll(
      int amount, std::string description);

  /**
   * @brief Factory for the spec card "Nyaleg". pay @p amount to each
   *        non-bankrupt opponent. Bankrupt opponents are skipped; if the
   *        payer cannot cover all payments they go bankrupt.
   * @param amount Funds transferred from the executing player to each opponent.
   * @param description Card flavour text.
   * @note Spec card from `Spesifikasi.md:480`.
   */
  static std::unique_ptr<ActionCard> makePayToAll(int amount,
                                                  std::string description);

 private:
  std::function<void(Player&, GameContext&)>
      effect_;  // Extension (not in spec).
};

}  // namespace core
