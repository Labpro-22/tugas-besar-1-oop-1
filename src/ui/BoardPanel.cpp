#include "ui/BoardPanel.hpp"

#include <SFML/Graphics/Color.hpp>
#include <stdexcept>

#include "ui/Color.hpp"
#include "ui/Constants.hpp"
#include "ui/Widgets.hpp"

namespace ui {

BoardPanel::BoardPanel()
    : Panel({dim::sideWidth, 0}, {dim::height, dim::height},
            color::light::surface::gameBoardBase) {
  if (regTileNum_ < 3 || regTileNum_ > 14)
    throw std::runtime_error("Invalid board configuration.");
}

BoardPanel::~BoardPanel() = default;

}  // namespace ui
