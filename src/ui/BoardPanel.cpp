#include "ui/BoardPanel.hpp"

#include <SFML/Graphics/Color.hpp>
#include <stdexcept>

#include "ui/Color.hpp"
#include "ui/Constants.hpp"
#include "ui/Widgets.hpp"

namespace ui {

BoardPanel::BoardPanel()
    : Panel({size::sideWidth, 0}, {size::height, size::height},
            component::board) {
  if (regTileNum_ < 3 || regTileNum_ > 14)
    throw std::runtime_error("Invalid board configuration.");
}

BoardPanel::~BoardPanel() = default;

}  // namespace ui
