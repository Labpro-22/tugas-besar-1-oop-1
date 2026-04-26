#pragma once

#include <vector>

namespace core {
class Player;
class Property;
class ActionCard;
} // namespace core

namespace logic {

struct AuctionResult {
    core::Player* winner;
    int finalBid;
};

class UIInputMediator {
public:
    virtual ~UIInputMediator() = default;

    virtual core::Property* selectFestivalProperty(core::Player& player) = 0;

    virtual core::Property* offerPropertyUI(core::Player& player, core::Property& property) = 0;

    virtual bool chooseTaxMethod(core::Player& player, int flatAmount, int percentageAmount) = 0;

    virtual AuctionResult runAuction(core::Property* property, const std::vector<core::Player*>& eligiblePlayers) = 0;

    virtual core::ActionCard* selectCardToDrop(core::Player& player) = 0;
};

} // namespace logic