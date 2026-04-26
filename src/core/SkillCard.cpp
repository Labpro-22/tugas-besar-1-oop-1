#include "core/SkillCard.hpp"

#include <utility>

namespace core {

SkillCard::SkillCard(std::string description)
    : ActionCard(std::move(description)) {}

}
