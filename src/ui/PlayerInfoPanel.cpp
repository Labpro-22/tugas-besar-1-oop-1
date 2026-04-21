#include "ui/PlayerInfoPanel.hpp"

#include "ui/Color.hpp"
#include "ui/Constants.hpp"

namespace ui {

PlayerInfoPanel::PlayerInfoPanel()
    : Panel({size::sideWidth + size::height, 0},
            {size::sideWidth, size::height}, component::sideBar) {}

PlayerInfoPanel::~PlayerInfoPanel() = default;

}  // namespace ui
