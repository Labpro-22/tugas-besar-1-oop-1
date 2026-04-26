#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <memory>
#include <string>
#include <utility>

#include "core/card/Card.hpp"
#include "core/player/Player.hpp"
#include "ui/component/Color.hpp"
#include "ui/component/Constants.hpp"
#include "ui/component/Style.hpp"
#include "ui/component/Widgets.hpp"
#include "ui/dialog/DialogManager.hpp"
#include "ui/dialog/DialogManagerInternal.hpp"

namespace ui {

std::pair<int, int> DialogManager::promptDiceOverride() {
  int dieOne = 1;
  int dieTwo = 1;
  bool done = false;

  auto overlay = buildModalOverlay(
      {layout::dialog::modalWidth, layout::dialog::modalHeight * 0.6f});
  Panel* dialog = static_cast<Panel*>(overlay->getChild("dialog"));
  const float modalHeight = layout::dialog::modalHeight * 0.6f;

  addTitleBand(*dialog, "ATUR DADU", layout::dialog::modalWidth);
  addCloseButton(*dialog, false, &done);

  const LabelStyle numberStyle(typography::title, palette::black,
                               typography::titleStyle, HorizontalAlign::Center,
                               VerticalAlign::Middle, {0, 0}, false,
                               typography::logBody);

  const ButtonStyle spinnerStyle(
      palette::darkGrey, palette::lightGrey, palette::darkGrey,
      component::button::disabled, palette::black, size::lineThickness,
      LabelStyle(typography::buttonPrimary, palette::white,
                 typography::buttonStyle, HorizontalAlign::Center,
                 VerticalAlign::Middle, {0, 0}, false, typography::logBody));

  const float spinnerButtonWidth = layout::dialog::buttonHeight;
  const float spinnerLabelWidth = dialog_detail::buttonWidth() -
                                  2.0f * spinnerButtonWidth -
                                  layout::dialog::rowGap;
  const float spinnerY = layout::dialog::titleHeight + layout::dialog::padding;

  Label* dieOneLabel = nullptr;
  Label* dieTwoLabel = nullptr;

  auto addSpinner = [&](float x, int& value, Label*& outputLabel) {
    auto minusButton = std::make_unique<Button>(
        "-", font_, sf::Vector2f{x, spinnerY},
        sf::Vector2f{spinnerButtonWidth, layout::dialog::buttonHeight},
        spinnerStyle, [&value, &outputLabel]() {
          value = std::max(1, value - 1);
          if (outputLabel) outputLabel->setText(std::to_string(value));
        });

    auto numberLabel = std::make_unique<Label>(
        std::to_string(value), font_,
        sf::Vector2f{x + spinnerButtonWidth + layout::dialog::rowGap * 0.5f,
                     spinnerY},
        sf::Vector2f{spinnerLabelWidth, layout::dialog::buttonHeight},
        numberStyle);
    outputLabel = numberLabel.get();

    auto plusButton = std::make_unique<Button>(
        "+", font_,
        sf::Vector2f{x + spinnerButtonWidth + layout::dialog::rowGap * 0.5f +
                         spinnerLabelWidth + layout::dialog::rowGap * 0.5f,
                     spinnerY},
        sf::Vector2f{spinnerButtonWidth, layout::dialog::buttonHeight},
        spinnerStyle, [&value, &outputLabel]() {
          value = std::min(6, value + 1);
          if (outputLabel) outputLabel->setText(std::to_string(value));
        });

    dialog->addChild(std::move(minusButton));
    dialog->addChild(std::move(numberLabel));
    dialog->addChild(std::move(plusButton));
  };

  addSpinner(layout::dialog::padding, dieOne, dieOneLabel);
  addSpinner(layout::dialog::padding + dialog_detail::buttonWidth() +
                 layout::dialog::rowGap,
             dieTwo, dieTwoLabel);

  const float buttonY =
      modalHeight - layout::dialog::buttonHeight - layout::dialog::padding;

  const ButtonStyle okStyle(
      component::button::confirm, accent::lightGreen, accent::darkGreen,
      component::button::disabled, sf::Color::Transparent, 0,
      LabelStyle(typography::buttonPrimary, palette::white,
                 typography::buttonStyle, HorizontalAlign::Center,
                 VerticalAlign::Middle, {0, 0}, false, typography::logBody));

  dialog->addChild(std::make_unique<Button>(
      "OK", font_, sf::Vector2f{layout::dialog::padding, buttonY},
      sf::Vector2f{layout::dialog::modalWidth - 2.0f * layout::dialog::padding,
                   layout::dialog::buttonHeight},
      okStyle, [&done]() { done = true; }));

  runModalLoop(*overlay, [&done]() { return done; });
  return {dieOne, dieTwo};
}

int DialogManager::promptCardChoice(const core::Player& player) {
  const auto hand = player.getHeldCards();
  if (hand.empty()) return 0;

  int selectedIndex = 0;
  bool done = false;

  auto overlay = buildModalOverlay(
      {layout::dialog::modalWidth, layout::dialog::modalHeight * 0.55f});
  Panel* dialog = static_cast<Panel*>(overlay->getChild("dialog"));
  const float modalHeight = layout::dialog::modalHeight * 0.55f;

  addTitleBand(*dialog, "GUNAKAN KARTU", layout::dialog::modalWidth);
  addCloseButton(*dialog, false, &done);

  const float slotWidth =
      (layout::dialog::modalWidth - 2.0f * layout::dialog::padding -
       static_cast<float>(hand.size() - 1) * layout::dialog::rowGap) /
      static_cast<float>(hand.size());
  const float slotY = layout::dialog::titleHeight + layout::dialog::padding;

  const ButtonStyle cardStyle(
      palette::darkGrey, palette::lightGrey, palette::darkGrey,
      component::button::disabled, palette::black, size::lineThickness,
      LabelStyle(
          typography::buttonSecondary, palette::white, typography::regular,
          HorizontalAlign::Center, VerticalAlign::Middle,
          {layout::dialog::padding * 0.3f, layout::dialog::padding * 0.2f},
          true, typography::logBody));

  for (size_t i = 0; i < hand.size(); ++i) {
    const int index = static_cast<int>(i);
    const float x =
        layout::dialog::padding +
        static_cast<float>(i) * (slotWidth + layout::dialog::rowGap);

    dialog->addChild(std::make_unique<Button>(
        hand[i]->getDescription(), font_, sf::Vector2f{x, slotY},
        sf::Vector2f{slotWidth, modalHeight - slotY - layout::dialog::padding},
        cardStyle, [&selectedIndex, &done, index]() {
          selectedIndex = index;
          done = true;
        }));
  }

  runModalLoop(*overlay, [&done]() { return done; });
  return selectedIndex;
}

}  // namespace ui
