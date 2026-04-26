#include <SFML/Graphics/Color.hpp>
#include <memory>
#include <string>

#include "core/Property.hpp"
#include "core/player/Player.hpp"
#include "ui/component/Color.hpp"
#include "ui/component/Constants.hpp"
#include "ui/component/Style.hpp"
#include "ui/component/Widgets.hpp"
#include "ui/dialog/DialogManager.hpp"
#include "ui/dialog/DialogManagerInternal.hpp"

namespace ui {

void DialogManager::showDeed(const core::Property& property) {
  bool done = false;

  auto overlay = buildModalOverlay(
      {layout::dialog::modalWidth, layout::dialog::modalHeight});
  Panel* dialog = static_cast<Panel*>(overlay->getChild("dialog"));

  addTitleBand(*dialog, "SERTIFIKAT PROPERTI", layout::dialog::modalWidth);

  const sf::Vector2f deedOrigin{
      layout::dialog::padding,
      layout::dialog::titleHeight + layout::dialog::padding};
  const sf::Vector2f deedSize{
      layout::dialog::modalWidth - 2.0f * layout::dialog::padding,
      layout::dialog::modalHeight - layout::dialog::titleHeight -
          3.0f * layout::dialog::padding - layout::dialog::buttonHeight};
  buildDeedPanel(*dialog, property, deedOrigin, deedSize);

  const float buttonY = layout::dialog::modalHeight -
                        layout::dialog::buttonHeight - layout::dialog::padding;

  const ButtonStyle closeStyle(
      palette::black, palette::darkGrey, palette::black,
      component::button::disabled, sf::Color::Transparent, 0,
      LabelStyle(typography::buttonPrimary, palette::white,
                 typography::buttonStyle, HorizontalAlign::Center,
                 VerticalAlign::Middle, {0, 0}, false, typography::logBody));

  dialog->addChild(std::make_unique<Button>(
      "TUTUP", font_, sf::Vector2f{layout::dialog::padding, buttonY},
      sf::Vector2f{layout::dialog::modalWidth - 2.0f * layout::dialog::padding,
                   layout::dialog::buttonHeight},
      closeStyle, [&done]() { done = true; }));

  runModalLoop(*overlay, [&done]() { return done; });
}

void DialogManager::showOwnedProperties(const core::Player& player) {
  bool done = false;

  auto overlay = buildModalOverlay(
      {layout::dialog::modalWidth, layout::dialog::modalHeight});
  Panel* dialog = static_cast<Panel*>(overlay->getChild("dialog"));

  addTitleBand(*dialog, "PROPERTI: " + player.getName(),
               layout::dialog::modalWidth);

  const auto& properties = player.getOwnedProperties();
  const LabelStyle rowStyle(
      typography::dialogBody, palette::white, typography::regular,
      HorizontalAlign::Left, VerticalAlign::Middle,
      {layout::dialog::padding * 0.4f, 0}, false, typography::logBody);

  float y = layout::dialog::titleHeight + layout::dialog::padding * 0.5f;
  const float rowHeight = layout::dialog::buttonHeight * 0.65f;

  for (const core::Property* property : properties) {
    if (!property) continue;
    dialog->addChild(std::make_unique<Label>(
        property->getName() + "   " +
            dialog_detail::formatMoney(property->getPrice()),
        font_, sf::Vector2f{0, y},
        sf::Vector2f{layout::dialog::modalWidth, rowHeight}, rowStyle));
    y += rowHeight + layout::dialog::rowGap * 0.5f;
  }

  if (properties.empty()) {
    dialog->addChild(std::make_unique<Label>(
        "Tidak ada properti.", font_,
        sf::Vector2f{0, layout::dialog::titleHeight + layout::dialog::padding},
        sf::Vector2f{layout::dialog::modalWidth, layout::dialog::buttonHeight},
        rowStyle));
  }

  const float buttonY = layout::dialog::modalHeight -
                        layout::dialog::buttonHeight - layout::dialog::padding;

  const ButtonStyle closeStyle(
      palette::black, palette::darkGrey, palette::black,
      component::button::disabled, sf::Color::Transparent, 0,
      LabelStyle(typography::buttonPrimary, palette::white,
                 typography::buttonStyle, HorizontalAlign::Center,
                 VerticalAlign::Middle, {0, 0}, false, typography::logBody));

  dialog->addChild(std::make_unique<Button>(
      "TUTUP", font_, sf::Vector2f{layout::dialog::padding, buttonY},
      sf::Vector2f{layout::dialog::modalWidth - 2.0f * layout::dialog::padding,
                   layout::dialog::buttonHeight},
      closeStyle, [&done]() { done = true; }));

  runModalLoop(*overlay, [&done]() { return done; });
}

void DialogManager::showBankruptcy(const core::Player& player) {
  bool done = false;

  auto overlay = buildModalOverlay(
      {layout::dialog::modalWidth, layout::dialog::modalHeight * 0.45f});
  Panel* dialog = static_cast<Panel*>(overlay->getChild("dialog"));
  const float modalHeight = layout::dialog::modalHeight * 0.45f;

  addTitleBand(*dialog, "BANGKRUT", layout::dialog::modalWidth);

  const LabelStyle messageStyle(
      typography::dialogBody, palette::white, typography::regular,
      HorizontalAlign::Center, VerticalAlign::Middle,
      {layout::dialog::padding * 0.4f, 0}, false, typography::logBody);

  dialog->addChild(std::make_unique<Label>(
      player.getName() + " telah bangkrut!", font_,
      sf::Vector2f{
          0, layout::dialog::titleHeight + layout::dialog::padding * 0.5f},
      sf::Vector2f{layout::dialog::modalWidth, layout::dialog::buttonHeight},
      messageStyle));

  const float buttonY =
      modalHeight - layout::dialog::buttonHeight - layout::dialog::padding;

  const ButtonStyle okStyle(
      palette::black, palette::darkGrey, palette::black,
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
}

void DialogManager::showAuctionResult(const core::Player* winner, int amount) {
  bool done = false;

  auto overlay = buildModalOverlay(
      {layout::dialog::modalWidth, layout::dialog::modalHeight * 0.45f});
  Panel* dialog = static_cast<Panel*>(overlay->getChild("dialog"));
  const float modalHeight = layout::dialog::modalHeight * 0.45f;

  addTitleBand(*dialog, "HASIL LELANG", layout::dialog::modalWidth);

  const std::string message = winner ? winner->getName() + " menang dengan " +
                                           dialog_detail::formatMoney(amount)
                                     : "Tidak ada pemenang.";

  const LabelStyle messageStyle(
      typography::dialogBody, palette::white, typography::regular,
      HorizontalAlign::Center, VerticalAlign::Middle,
      {layout::dialog::padding * 0.4f, 0}, false, typography::logBody);

  dialog->addChild(std::make_unique<Label>(
      message, font_,
      sf::Vector2f{
          0, layout::dialog::titleHeight + layout::dialog::padding * 0.5f},
      sf::Vector2f{layout::dialog::modalWidth, layout::dialog::buttonHeight},
      messageStyle));

  const float buttonY =
      modalHeight - layout::dialog::buttonHeight - layout::dialog::padding;

  const ButtonStyle okStyle(
      palette::black, palette::darkGrey, palette::black,
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
}

}  // namespace ui
