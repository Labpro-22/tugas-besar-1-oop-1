#include "ui/ActionPanel.hpp"

#include "ui/Color.hpp"
#include "ui/Constants.hpp"
#include "ui/Widgets.hpp"

namespace ui {

ActionPanel::ActionPanel()
    : Panel({0, 0}, {size::sideWidth, size::height}, component::sideBar) {}

ActionPanel::~ActionPanel() = default;

}  // namespace ui
