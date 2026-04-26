#include "core/ChanceCard.hpp"

#include "core/GameContext.hpp"
#include "core/Player.hpp"

#include <utility>

namespace core {

ChanceCard::ChanceCard(std::string description, std::function<void(Player&, GameContext&)> effect)
        : ActionCard(std::move(description)), effect_(std::move(effect)) {}

void ChanceCard::execute(Player& player, GameContext& context) {
    if (effect_) {
        effect_(player, context);
    }
}

std::string ChanceCard::getCardType() const { return "Chance"; }

std::unique_ptr<ActionCard> ChanceCard::makeMoveBack(int spaces, std::string description) {
    return std::unique_ptr<ActionCard>(new ChanceCard(std::move(description), [spaces](Player& player, GameContext& ctx) {
            const int n = ctx.getBoardSize();
            if (n <= 0) {
                return;
            }
            // TODO: out-of-spec - bypasses Game::moveCurrentPlayer / onPassed / pass-Go handling.
            int pos = player.getPosition() - spaces;
            pos %= n;
            if (pos < 0) {
                pos += n;
            }
            player.setPosition(pos);
        }));
}

std::unique_ptr<ActionCard> ChanceCard::makePayBank(int amount, std::string description) {
    return std::unique_ptr<ActionCard>(new ChanceCard(std::move(description), [amount](Player& player, GameContext& ctx) {
            // PBM tax is a flat, unconditional bank payment, matching the old "pay bank" semantics.
            ctx.chargeTax(player, amount, 0, TaxType::PBM);
        }));
}

std::unique_ptr<ActionCard> ChanceCard::makeGoToJail(std::string description) {
    return std::unique_ptr<ActionCard>(new ChanceCard(std::move(description), [](Player& player, GameContext& ctx) {
            ctx.sendToJail(player);
        }));
}

std::unique_ptr<ActionCard> ChanceCard::makeAdvanceTo(int index, std::string description) {
    return std::unique_ptr<ActionCard>(new ChanceCard(std::move(description), [index](Player& player, GameContext& ctx) {
            const int n = ctx.getBoardSize();
            if (n <= 0) {
                return;
            }
            // TODO: out-of-spec - bypasses Game::moveCurrentPlayer / onPassed / pass-Go handling.
            int pos = index % n;
            if (pos < 0) {
                pos += n;
            }
            player.setPosition(pos);
        }));
}

}
