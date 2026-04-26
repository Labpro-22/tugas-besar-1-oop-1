#include "core/CommunityChestCard.hpp"

#include "core/GameContext.hpp"
#include "core/Player.hpp"

#include <utility>

namespace core {

CommunityChestCard::CommunityChestCard(std::string description, std::function<void(Player&, GameContext&)> effect)
        : ActionCard(std::move(description)), effect_(std::move(effect)) {}

void CommunityChestCard::execute(Player& player, GameContext& context) {
    if (effect_) {
        effect_(player, context);
    }
}

std::string CommunityChestCard::getCardType() const {
    return "CommunityChest";
}

std::unique_ptr<ActionCard> CommunityChestCard::makePayBank(int amount, std::string description) {
    return std::unique_ptr<ActionCard>(new CommunityChestCard(std::move(description),  [amount](Player& player, GameContext& ctx) {
            // PBM tax is a flat, unconditional bank payment, matching the old "pay bank" semantics.
            ctx.chargeTax(player, amount, 0, TaxType::PBM);
        }));
}

std::unique_ptr<ActionCard> CommunityChestCard::makeCollect(int amount, std::string description) {
    return std::unique_ptr<ActionCard>(new CommunityChestCard(std::move(description), [amount](Player& player, GameContext& ctx) {
            ctx.payPlayerFromBank(player, amount);
        }));
}

std::unique_ptr<ActionCard> CommunityChestCard::makeCollectFromAll(int amount, std::string description) {
    return std::unique_ptr<ActionCard>(new CommunityChestCard(std::move(description), [amount](Player& player, GameContext& ctx) {
            int collected = 0;
            for (Player* candidate : ctx.getPlayers()) {
                if (candidate == nullptr || candidate == &player || candidate->isBankrupted()) {
                    continue;
                }
                if (candidate->canAfford(amount)) {
                    *candidate -= amount;
                    collected += amount;
                } else {
                    collected += candidate->getBalance();
                    *candidate -= candidate->getBalance();
                    candidate->setBankrupted(true);
                }
            }
            if (collected > 0) {
                player += collected;
            }
        }));
}

std::unique_ptr<ActionCard> CommunityChestCard::makePayToAll(int amount, std::string description) {
    return std::unique_ptr<ActionCard>(new CommunityChestCard(std::move(description), [amount](Player& player, GameContext& ctx) {
            for (Player* candidate : ctx.getPlayers()) {
                if (candidate == nullptr || candidate == &player || candidate->isBankrupted()) {
                    continue;
                }
                if (player.canAfford(amount)) {
                    player -= amount;
                    *candidate += amount;
                } else {
                    const int leftover = player.getBalance();
                    if (leftover > 0) {
                        player -= leftover;
                        *candidate += leftover;
                    }
                    player.setBankrupted(true);
                    return;
                }
            }
        }));
}

}
