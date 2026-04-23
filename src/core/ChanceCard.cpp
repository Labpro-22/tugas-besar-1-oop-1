#include "core/ChanceCard.hpp"

#include "core/Player.hpp"
#include "logic/Bank.hpp"
#include "logic/Game.hpp"

#include <utility>

namespace core {

ChanceCard::ChanceCard(std::string description, std::function<void(Player&, logic::Game&)> effect)
        : ActionCard(std::move(description)), effect_(std::move(effect)) {}

void ChanceCard::execute(Player& player, logic::Game& game) {
    if (effect_) {
        effect_(player, game);
    }
}

std::string ChanceCard::getCardType() const { return "Chance"; }

std::unique_ptr<ActionCard> ChanceCard::makeMoveBack(int spaces, std::string description) {
    return std::unique_ptr<ActionCard>(new ChanceCard(std::move(description), [spaces](Player& player, logic::Game& game) {
            (void)player;
            game.moveCurrentPlayer(-spaces);
        }));
}

std::unique_ptr<ActionCard> ChanceCard::makePayBank(int amount, std::string description) {
    return std::unique_ptr<ActionCard>(new ChanceCard(std::move(description), [amount](Player& player, logic::Game& game) {
             player -= amount;
            game.bank().receive(amount);
        }));
}

std::unique_ptr<ActionCard> ChanceCard::makeGoToJail(std::string description) {
    return std::unique_ptr<ActionCard>(new ChanceCard(std::move(description), [](Player& player, logic::Game& game) {
            player.goToJail();
            const int jailIndex = game.board().getJailTileIndex();
            if (jailIndex >= 0) {
                player.setPosition(jailIndex);
            }
        }));
}

std::unique_ptr<ActionCard> ChanceCard::makeAdvanceTo(int index, std::string description) {
    return std::unique_ptr<ActionCard>(new ChanceCard( std::move(description), [index](Player& player, logic::Game& game) {
            (void)game;
            player.setPosition(index);
        }));
}

}
