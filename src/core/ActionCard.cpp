#include "core/ActionCard.hpp"
#include "core/CardDeck.hpp"

#include <utility>

namespace core {

ActionCard::ActionCard(std::string description)
    : description_(std::move(description)) {}

std::string ActionCard::getDescription() const { return description_; }

template class CardDeck<ActionCard>;

}
