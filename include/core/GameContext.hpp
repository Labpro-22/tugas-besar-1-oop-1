#pragma once
#include <string>

namespace core {
class Player;
class Property;

class GameContext {
public:
    virtual ~GameContext() = default;

    // Property 
    virtual void offerProperty(Player& p, Property& prop) = 0;
    virtual void chargeRent(Player& p, Property& prop) = 0;

    // Jail
    virtual void sendToJail(Player& p) = 0;

    // Tax
    virtual void chargeTax(Player& p, int rate, bool isPercentage) = 0;

    // Festival
    virtual void activateFestival(Player& p) = 0;

    // Cards
    virtual void drawChanceCard(Player& p) = 0;
    virtual void drawCommunityChestCard(Player& p) = 0;

    // Bank payments
    virtual void payPlayerFromBank(Player& p, int amount) = 0;

    // Logging
    virtual void logEvent(const std::string& action, Player& p, int value) = 0;
    virtual void logEvent(const std::string& action, Player& p,
                          Property& prop, int value) = 0;

    // Config getters tiles need
    virtual int getGoSalary() const = 0;
    virtual std::pair<int,int> getLastDiceRoll() const = 0;
};

} // namespace core