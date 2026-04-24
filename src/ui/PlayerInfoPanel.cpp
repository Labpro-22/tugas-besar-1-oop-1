#include "ui/PlayerInfoPanel.hpp"

#include "ui/component/Color.hpp"
#include "ui/component/Constants.hpp"

namespace ui {

PlayerInfoPanel::PlayerInfoPanel()
    : Panel(
          {size::sideWidth + size::height, 0}, {size::sideWidth, size::height},
          PanelStyle(component::sideBar, palette::black, size::lineThickness)) {
}

PlayerInfoPanel::~PlayerInfoPanel() = default;

}  // namespace ui
