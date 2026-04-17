#include "ui/PlayerInfoPanel.hpp"

#include "ui/Color.hpp"
#include "ui/Constants.hpp"

namespace ui {

PlayerInfoPanel::PlayerInfoPanel()
    : Panel({dim::sideWidth + dim::height, 0}, {dim::sideWidth, dim::height},
            color::light::surface::background) {}

PlayerInfoPanel::~PlayerInfoPanel() = default;

}  // namespace ui
