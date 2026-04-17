#pragma once

#include "ui/Widgets.hpp"

namespace ui {

class BoardPanel : public Panel {
 public:
  BoardPanel();
  ~BoardPanel() override;

 private:
  int regTileNum_ = 9; /**< Number of regular tile size. [3..14] for 20x20 up to
                      60x60 board. Default is 9 (40x40) */
};

}  // namespace ui
