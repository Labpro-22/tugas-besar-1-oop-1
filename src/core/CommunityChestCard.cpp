#include "core/card/CommunityChestCard.hpp"

#include <utility>

#include "core/GameContext.hpp"
#include "core/Player.hpp"

namespace core {

CommunityChestCard::CommunityChestCard(
    std::string description, std::function<void(Player&, GameContext&)> effect)
    : ActionCard(std::move(description)), effect_(std::move(effect)) {}

void CommunityChestCard::execute(Player& player, GameContext& context) {
  if (effect_) {
    effect_(player, context);
  }
}

std::string CommunityChestCard::getCardType() const { return "CommunityChest"; }

std::unique_ptr<ActionCard> CommunityChestCard::makePayBank(
    int amount, std::string description) {
  return std::unique_ptr<ActionCard>(new CommunityChestCard(
      std::move(description), [amount](Player& player, GameContext& ctx) {
        // PBM tax is a flat, unconditional bank payment, matching the old "pay
        // bank" semantics.
        ctx.chargeTax(player, amount, 0, TaxType::PBM);
      }));
}

std::unique_ptr<ActionCard> CommunityChestCard::makeCollect(
    int amount, std::string description) {
  return std::unique_ptr<ActionCard>(new CommunityChestCard(
      std::move(description), [amount](Player& player, GameContext& ctx) {
        ctx.payPlayerFromBank(player, amount);
      }));
}

}  // namespace core
