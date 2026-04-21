#pragma once
#include <vector>
namespace core {
  class Player;
  class Property;
}

using namespace std;
namespace logic {
  class Game {
  public:
    Game();
    int getLastDiceRoll() const;
    vector<core::Property *> getPropertiesOwnedBy(core::Player *p) const;

  private:
    int lastDiceRoll_; // state lainnya nanti ditambahin sesuai kebutuhan
  };

} // namespace logic