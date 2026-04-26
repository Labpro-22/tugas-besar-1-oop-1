#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core/Property.hpp"
#include "core/card/Card.hpp"
#include "core/player/Player.hpp"
#include "ui/component/Color.hpp"
#include "ui/component/Constants.hpp"
#include "ui/component/Style.hpp"
#include "ui/component/Widgets.hpp"
#include "ui/dialog/DialogManager.hpp"
#include "ui/dialog/DialogManagerInternal.hpp"

namespace ui {

core::Property* DialogManager::selectFestivalProperty(core::Player& player) {
  const auto& properties = player.getOwnedProperties();
  if (properties.empty()) return nullptr;

  core::Property* selected = nullptr;
  bool done = false;

  auto overlay = buildModalOverlay(
      {layout::dialog::modalWidth, layout::dialog::modalHeight});
  Panel* dialog = static_cast<Panel*>(overlay->getChild("dialog"));

  addTitleBand(*dialog, "FESTIVAL: PILIH PROPERTI", layout::dialog::modalWidth);
    addCloseButton(*dialog, false, &done);

  const LabelStyle infoStyle(
      typography::dialogBody, palette::black, typography::regular,
      HorizontalAlign::Center, VerticalAlign::Top,
      {layout::dialog::padding * 0.5f, layout::dialog::padding * 0.3f}, false,
      typography::logBody);

  dialog->addChild(std::make_unique<Label>(
      "Pilih properti untuk mendapat bonus festival:", font_,
      sf::Vector2f{
          0, layout::dialog::titleHeight + layout::dialog::padding * 0.4f},
      sf::Vector2f{layout::dialog::modalWidth,
                   layout::dialog::buttonHeight * 0.7f},
      infoStyle));

  const ButtonStyle propertyStyle(
      palette::darkGrey, palette::lightGrey, palette::darkGrey,
      component::button::disabled, palette::black, size::lineThickness,
      LabelStyle(typography::buttonPrimary, palette::white,
                 typography::buttonStyle, HorizontalAlign::Center,
                 VerticalAlign::Middle, {layout::dialog::padding * 0.3f, 0},
                 true, typography::logBody));

  const float startY = layout::dialog::titleHeight +
                       layout::dialog::buttonHeight * 0.7f +
                       layout::dialog::padding;
  const float fullButtonWidth =
      layout::dialog::modalWidth - 2.0f * layout::dialog::padding;

  for (size_t i = 0; i < properties.size() && i < 8; ++i) {
    core::Property* property = properties[i];
    if (!property) continue;

    const float y =
        startY + static_cast<float>(i) *
                     (layout::dialog::buttonHeight + layout::dialog::rowGap);

    dialog->addChild(std::make_unique<Button>(
        property->getName() + "  " +
            dialog_detail::formatMoney(property->getPrice()),
        font_, sf::Vector2f{layout::dialog::padding, y},
        sf::Vector2f{fullButtonWidth, layout::dialog::buttonHeight},
        propertyStyle, [&selected, &done, property]() {
          selected = property;
          done = true;
        }));
  }

  const ButtonStyle cancelStyle(
      component::button::cancel, accent::pink, accent::darkRed,
      component::button::disabled, sf::Color::Transparent, 0,
      LabelStyle(typography::buttonPrimary, palette::white,
                 typography::buttonStyle, HorizontalAlign::Center,
                 VerticalAlign::Middle, {0, 0}, false, typography::logBody));

  const float cancelY =
      startY + 8.0f * (layout::dialog::buttonHeight + layout::dialog::rowGap) +
      layout::dialog::padding;

  dialog->addChild(std::make_unique<Button>(
      "BATAL", font_, sf::Vector2f{layout::dialog::padding, cancelY},
      sf::Vector2f{fullButtonWidth, layout::dialog::buttonHeight}, cancelStyle,
      [&done]() { done = true; }));

  runModalLoop(*overlay, [&done]() { return done; });
  return selected;
}

bool DialogManager::offerProperty(core::Player& player,
                                  core::Property& property) {
  bool accept = false;
  bool done = false;

  auto overlay = buildModalOverlay(
      {layout::dialog::modalWidth, layout::dialog::modalHeight});
  Panel* dialog = static_cast<Panel*>(overlay->getChild("dialog"));

  addTitleBand(*dialog, "TAWARAN PROPERTI", layout::dialog::modalWidth);
    addCloseButton(*dialog, false, &done);

  const sf::Vector2f deedOrigin{
      layout::dialog::padding,
      layout::dialog::titleHeight + layout::dialog::padding};
  const sf::Vector2f deedSize{
      layout::dialog::deedWidth,
      layout::dialog::deedHeight - layout::dialog::titleHeight -
          2.0f * layout::dialog::padding - layout::dialog::buttonHeight -
          layout::dialog::rowGap};
  buildDeedPanel(*dialog, property, deedOrigin, deedSize);

  const LabelStyle infoStyle(
      typography::dialogBody, palette::black, typography::regular,
      HorizontalAlign::Left, VerticalAlign::Top,
      {layout::dialog::padding * 0.4f, layout::dialog::padding * 0.3f}, false,
      typography::logBody);

  dialog->addChild(std::make_unique<Label>(
      player.getName() + "  " + dialog_detail::formatMoney(player.getBalance()),
      font_,
      sf::Vector2f{layout::dialog::padding, layout::dialog::titleHeight +
                                                deedSize.y +
                                                2.0f * layout::dialog::padding},
      sf::Vector2f{layout::dialog::modalWidth - 2.0f * layout::dialog::padding,
                   layout::dialog::buttonHeight * 0.7f},
      infoStyle));

  const float buttonY = layout::dialog::modalHeight -
                        layout::dialog::buttonHeight - layout::dialog::padding;

  const ButtonStyle buyStyle(
      component::button::confirm, accent::lightGreen, accent::darkGreen,
      component::button::disabled, sf::Color::Transparent, 0,
      LabelStyle(typography::buttonPrimary, palette::white,
                 typography::buttonStyle, HorizontalAlign::Center,
                 VerticalAlign::Middle, {0, 0}, false, typography::logBody));

  const ButtonStyle auctionStyle(
      component::button::cancel, accent::pink, accent::darkRed,
      component::button::disabled, sf::Color::Transparent, 0,
      LabelStyle(typography::buttonPrimary, palette::white,
                 typography::buttonStyle, HorizontalAlign::Center,
                 VerticalAlign::Middle, {0, 0}, false, typography::logBody));

  dialog->addChild(std::make_unique<Button>(
      "BELI", font_, sf::Vector2f{layout::dialog::padding, buttonY},
      sf::Vector2f{dialog_detail::buttonWidth(), layout::dialog::buttonHeight},
      buyStyle, [&accept, &done]() {
        accept = true;
        done = true;
      }));

  dialog->addChild(std::make_unique<Button>(
      "LELANG", font_,
      sf::Vector2f{layout::dialog::padding + dialog_detail::buttonWidth() +
                       layout::dialog::rowGap,
                   buttonY},
      sf::Vector2f{dialog_detail::buttonWidth(), layout::dialog::buttonHeight},
      auctionStyle, [&accept, &done]() {
        accept = false;
        done = true;
      }));

  runModalLoop(*overlay, [&done]() { return done; });
  return accept;
}

bool DialogManager::chooseTaxMethod(core::Player& player, int flatAmount,
                                    int percentageAmount) {
  bool usePercent = false;
  bool done = false;

  auto overlay = buildModalOverlay(
      {layout::dialog::modalWidth, layout::dialog::modalHeight * 0.5f});
  Panel* dialog = static_cast<Panel*>(overlay->getChild("dialog"));
  const float modalHeight = layout::dialog::modalHeight * 0.5f;

  addTitleBand(*dialog, "PILIH METODE PAJAK", layout::dialog::modalWidth);
    addCloseButton(*dialog, false, &done);

  const LabelStyle infoStyle(
      typography::dialogBody, palette::black, typography::regular,
      HorizontalAlign::Center, VerticalAlign::Middle,
      {layout::dialog::padding * 0.4f, 0}, false, typography::logBody);

  dialog->addChild(std::make_unique<Label>(
      player.getName() + " - pilih cara bayar pajak:", font_,
      sf::Vector2f{
          0, layout::dialog::titleHeight + layout::dialog::padding * 0.5f},
      sf::Vector2f{layout::dialog::modalWidth,
                   layout::dialog::buttonHeight * 0.7f},
      infoStyle));

  const float buttonY =
      modalHeight - layout::dialog::buttonHeight - layout::dialog::padding;

  const ButtonStyle flatStyle(
      accent::darkBlue, accent::blue50, accent::darkBlue,
      component::button::disabled, sf::Color::Transparent, 0,
      LabelStyle(typography::buttonPrimary, palette::white,
                 typography::buttonStyle, HorizontalAlign::Center,
                 VerticalAlign::Middle, {0, 0}, false, typography::logBody));

  const ButtonStyle percentStyle(
      accent::purple, accent::lavender, accent::purple,
      component::button::disabled, sf::Color::Transparent, 0,
      LabelStyle(typography::buttonPrimary, palette::white,
                 typography::buttonStyle, HorizontalAlign::Center,
                 VerticalAlign::Middle, {0, 0}, false, typography::logBody));

  dialog->addChild(std::make_unique<Button>(
      "TETAP " + dialog_detail::formatMoney(flatAmount), font_,
      sf::Vector2f{layout::dialog::padding, buttonY},
      sf::Vector2f{dialog_detail::buttonWidth(), layout::dialog::buttonHeight},
      flatStyle, [&usePercent, &done]() {
        usePercent = false;
        done = true;
      }));

  dialog->addChild(std::make_unique<Button>(
      "PERSEN " + dialog_detail::formatMoney(percentageAmount), font_,
      sf::Vector2f{layout::dialog::padding + dialog_detail::buttonWidth() +
                       layout::dialog::rowGap,
                   buttonY},
      sf::Vector2f{dialog_detail::buttonWidth(), layout::dialog::buttonHeight},
      percentStyle, [&usePercent, &done]() {
        usePercent = true;
        done = true;
      }));

  runModalLoop(*overlay, [&done]() { return done; });
  return usePercent;
}

std::pair<core::Player*, int> DialogManager::runAuction(
    core::Property* property,
    const std::vector<core::Player*>& eligiblePlayers) {
  if (!property || eligiblePlayers.empty()) return {nullptr, 0};

  std::vector<core::Player*> remaining = eligiblePlayers;
  int currentBid = 0;
  core::Player* currentWinner = nullptr;
  bool done = false;
  size_t activeIndex = 0;

  auto rebuild = [&]() -> std::unique_ptr<Panel> {
    auto overlay = buildModalOverlay(
        {layout::dialog::modalWidth, layout::dialog::modalHeight});
    Panel* dialog = static_cast<Panel*>(overlay->getChild("dialog"));

    addTitleBand(*dialog, "LELANG: " + property->getName(),
                 layout::dialog::modalWidth);
    addCloseButton(*dialog, false, &done);

    const LabelStyle bidStyle(typography::title, palette::black,
                              typography::titleStyle, HorizontalAlign::Center,
                              VerticalAlign::Middle, {0, 0}, false,
                              typography::logBody);

    const std::string bidText = currentBid == 0
                                    ? "Belum ada tawaran"
                                    : dialog_detail::formatMoney(currentBid);
    dialog->addChild(std::make_unique<Label>(
        bidText, font_,
        sf::Vector2f{
            0, layout::dialog::titleHeight + layout::dialog::padding * 0.3f},
        sf::Vector2f{layout::dialog::modalWidth, layout::dialog::buttonHeight},
        bidStyle));

    if (remaining.empty()) {
      done = true;
      return overlay;
    }

    core::Player* bidder = remaining[activeIndex % remaining.size()];

    const LabelStyle promptStyle(
        typography::dialogBody, palette::black, typography::regular,
        HorizontalAlign::Center, VerticalAlign::Top,
        {layout::dialog::padding * 0.4f, layout::dialog::padding * 0.2f}, false,
        typography::logBody);

    dialog->addChild(std::make_unique<Label>(
        bidder->getName() + " (M" + std::to_string(bidder->getBalance()) + ")",
        font_,
        sf::Vector2f{0, layout::dialog::titleHeight +
                            layout::dialog::buttonHeight +
                            layout::dialog::padding * 0.5f},
        sf::Vector2f{layout::dialog::modalWidth,
                     layout::dialog::buttonHeight * 0.7f},
        promptStyle));

    const float buttonY = layout::dialog::modalHeight -
                          layout::dialog::buttonHeight -
                          layout::dialog::padding;
    const int nextBid = currentBid + std::max(1, currentBid / 10 + 50);

    const ButtonStyle raiseStyle(
        component::button::confirm, accent::lightGreen, accent::darkGreen,
        component::button::disabled, sf::Color::Transparent, 0,
        LabelStyle(typography::buttonPrimary, palette::white,
                   typography::buttonStyle, HorizontalAlign::Center,
                   VerticalAlign::Middle, {0, 0}, false, typography::logBody));

    const ButtonStyle passStyle(
        component::button::cancel, accent::pink, accent::darkRed,
        component::button::disabled, sf::Color::Transparent, 0,
        LabelStyle(typography::buttonPrimary, palette::white,
                   typography::buttonStyle, HorizontalAlign::Center,
                   VerticalAlign::Middle, {0, 0}, false, typography::logBody));

    auto raiseButton = std::make_unique<Button>(
        "TAWAR " + dialog_detail::formatMoney(nextBid), font_,
        sf::Vector2f{layout::dialog::padding, buttonY},
        sf::Vector2f{dialog_detail::buttonWidth(),
                     layout::dialog::buttonHeight},
        raiseStyle, [&, nextBid, bidder]() {
          if (bidder->canAfford(nextBid)) {
            currentBid = nextBid;
            currentWinner = bidder;
            activeIndex++;
            if (remaining.size() <= 1) done = true;
          }
        });

    raiseButton->setActive(bidder->canAfford(nextBid));
    dialog->addChild(std::move(raiseButton));

    dialog->addChild(std::make_unique<Button>(
        "LEWATI", font_,
        sf::Vector2f{layout::dialog::padding + dialog_detail::buttonWidth() +
                         layout::dialog::rowGap,
                     buttonY},
        sf::Vector2f{dialog_detail::buttonWidth(),
                     layout::dialog::buttonHeight},
        passStyle, [&, bidder]() {
          remaining.erase(
              std::remove(remaining.begin(), remaining.end(), bidder),
              remaining.end());
          if (remaining.size() <= 1) {
            done = true;
          } else {
            activeIndex %= remaining.size();
          }
        }));

    return overlay;
  };

  while (!done) {
    auto overlay = rebuild();
    const size_t indexBefore = activeIndex;
    const size_t remainingBefore = remaining.size();

    runModalLoop(*overlay, [&] {
      return done || activeIndex != indexBefore ||
             remaining.size() != remainingBefore;
    });
  }

  return {currentWinner, currentBid};
}

core::ActionCard* DialogManager::selectCardToDrop(core::Player& player) {
  core::ActionCard* selected = nullptr;
  bool done = false;

  const auto hand = player.getHeldCards();
  if (hand.empty()) return nullptr;

  auto overlay = buildModalOverlay(
      {layout::dialog::modalWidth, layout::dialog::modalHeight * 0.55f});
  Panel* dialog = static_cast<Panel*>(overlay->getChild("dialog"));
  const float modalHeight = layout::dialog::modalHeight * 0.55f;

  addTitleBand(*dialog, "BUANG KARTU", layout::dialog::modalWidth);
    addCloseButton(*dialog, false, &done);

  const LabelStyle infoStyle(
      typography::dialogBody, palette::black, typography::regular,
      HorizontalAlign::Center, VerticalAlign::Top,
      {layout::dialog::padding * 0.4f, layout::dialog::padding * 0.3f}, false,
      typography::logBody);

  dialog->addChild(std::make_unique<Label>(
      "Tangan penuh. Pilih kartu yang dibuang:", font_,
      sf::Vector2f{
          0, layout::dialog::titleHeight + layout::dialog::padding * 0.3f},
      sf::Vector2f{layout::dialog::modalWidth,
                   layout::dialog::buttonHeight * 0.7f},
      infoStyle));

  const float slotWidth =
      (layout::dialog::modalWidth - 2.0f * layout::dialog::padding -
       static_cast<float>(hand.size() - 1) * layout::dialog::rowGap) /
      static_cast<float>(hand.size());
  const float slotY = layout::dialog::titleHeight +
                      layout::dialog::buttonHeight * 0.7f +
                      layout::dialog::padding;

  const ButtonStyle cardStyle(
      palette::darkGrey, palette::lightGrey, palette::darkGrey,
      component::button::disabled, palette::black, size::lineThickness,
      LabelStyle(
          typography::buttonSecondary, palette::white, typography::regular,
          HorizontalAlign::Center, VerticalAlign::Middle,
          {layout::dialog::padding * 0.3f, layout::dialog::padding * 0.2f},
          true, typography::logBody));

  for (size_t i = 0; i < hand.size(); ++i) {
    core::ActionCard* card = hand[i];
    const float x =
        layout::dialog::padding +
        static_cast<float>(i) * (slotWidth + layout::dialog::rowGap);

    dialog->addChild(std::make_unique<Button>(
        card->getDescription(), font_, sf::Vector2f{x, slotY},
        sf::Vector2f{slotWidth, modalHeight - slotY - layout::dialog::padding},
        cardStyle, [&selected, &done, card]() {
          selected = card;
          done = true;
        }));
  }

  runModalLoop(*overlay, [&done]() { return done; });
  return selected;
}

}  // namespace ui
