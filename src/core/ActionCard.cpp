#include "core/card/ActionCard.hpp"

#include <utility>

#include "core/card/CardDeck.hpp"

namespace core {

ActionCard::ActionCard(std::string description)
    : description_(std::move(description)) {}

std::string ActionCard::getDescription() const { return description_; }

template class CardDeck<ActionCard>;

}  // namespace core
