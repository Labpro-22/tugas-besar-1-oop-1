#include "ui/sidebar/PlayerInfoPanel.hpp"

#include "ui/component/Color.hpp"
#include "ui/component/Constants.hpp"

namespace ui {

PlayerInfoPanel::PlayerInfoPanel()
    : Panel(
          {size::sideWidth + size::height, 0}, {size::sideWidth, size::height},
          PanelStyle(component::sideBar, palette::black, size::lineThickness)) {
}

PlayerInfoPanel::~PlayerInfoPanel() = default;

void PlayerInfoPanel::setPlayers(const std::vector<temp::Player*>& players) {
  playerCards_.clear();
  playerCards_.reserve(players.size());

  const float x = position_.x + size::defaultMargin;
  const float y0 = position_.y + layout::playerCard::verticalSpacing;
  const float gap = layout::playerCard::verticalSpacing;

  for (size_t i = 0; i < players.size(); ++i) {
    auto card = std::make_unique<PlayerCard>(
        sf::Vector2f{x, y0 + i * (size::playerCardHeight + gap)},
        sf::Vector2f{size::playerCardWidth, size::playerCardHeight});
    card->setPlayer(*players[i]);
    playerCards_.push_back(std::move(card));
  }
}

void PlayerInfoPanel::handleEvent(sf::Event& event, sf::RenderWindow& window) {
  Panel::handleEvent(event, window);
  for (std::unique_ptr<PlayerCard>& card : playerCards_) {
    card->handleEvent(event, window);
  }
}

void PlayerInfoPanel::render(sf::RenderWindow& window) {
  Panel::render(window);
  for (std::unique_ptr<PlayerCard>& card : playerCards_) {
    card->render(window);
  }
}

void PlayerInfoPanel::update(sf::RenderWindow& window) {
  Panel::update(window);
  for (std::unique_ptr<PlayerCard>& card : playerCards_) {
    card->update(window);
  }
}

}  // namespace ui
