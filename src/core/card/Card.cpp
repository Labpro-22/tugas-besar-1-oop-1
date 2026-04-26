#include "core/card/Card.hpp"

#include <utility>

#include "core/card/CardDeck.hpp"

namespace core {

ActionCard::ActionCard(std::string description)
    : description_(std::move(description)) {}

std::string ActionCard::getDescription() const { return description_; }

SkillCard::SkillCard(std::string description)
    : ActionCard(std::move(description)) {}

template class CardDeck<ActionCard>;

}  // namespace core
