#include "core/COMStrategy.hpp"

namespace core {

int COMStrategy::suggestChoice(const std::string& /*context*/, int defaultIndex, int /*optionCount*/) const noexcept {
    return defaultIndex;
}

}
