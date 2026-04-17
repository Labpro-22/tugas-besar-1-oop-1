#include "ui/ActionPanel.hpp"

#include "ui/Color.hpp"
#include "ui/Constants.hpp"
#include "ui/Widgets.hpp"

namespace ui {

ActionPanel::ActionPanel()
    : Panel({0, 0}, {dim::sideWidth, dim::height},
            color::light::surface::background) {}

ActionPanel::~ActionPanel() = default;

}  // namespace ui
