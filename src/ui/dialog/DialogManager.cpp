#include "ui/dialog/DialogManager.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <functional>
#include <memory>
#include <string>

#include "core/Property.hpp"
#include "core/Tiles.hpp"
#include "ui/AssetsManager.hpp"
#include "ui/component/Color.hpp"
#include "ui/component/Constants.hpp"
#include "ui/component/Style.hpp"
#include "ui/component/Widgets.hpp"
#include "ui/dialog/DialogManagerInternal.hpp"

namespace ui::dialog_detail {

float buttonWidth() {
  return (layout::dialog::modalWidth - 2.0f * layout::dialog::padding -
          layout::dialog::rowGap) /
         2.0f;
}

sf::Color groupColor(core::ColorGroup group) {
  using CG = core::ColorGroup;
  switch (group) {
    case CG::BROWN:
      return ui::board::property::brown;
    case CG::LIGHT_BLUE:
      return ui::board::property::lightBlue;
    case CG::PINK:
      return ui::board::property::pink;
    case CG::ORANGE:
      return ui::board::property::orange;
    case CG::RED:
      return ui::board::property::red;
    case CG::YELLOW:
      return ui::board::property::yellow;
    case CG::GREEN:
      return ui::board::property::green;
    case CG::DARK_BLUE:
      return ui::board::property::darkBlue;
    case CG::GREY:
      return ui::board::property::grey;
    case CG::PURPLE:
      return ui::board::property::purple;
    case CG::LIGHT_GREEN:
      return ui::board::property::lightGreen;
    case CG::LAVENDER:
      return ui::board::property::lavender;
    case CG::WHITE:
      return ui::board::property::white;
    case CG::BLACK:
      return ui::board::property::black;
    default:
      return ui::board::property::defaultColor;
  }
}

sf::Color propertyBandColor(const core::Property& property) {
  if (property.getType() == core::PropertyTileType::STREET) {
    return groupColor(
        static_cast<const core::Street&>(property).getColorGroup());
  }
  return ui::board::property::defaultColor;
}

std::string formatMoney(int amount) { return "M" + std::to_string(amount); }

}  // namespace ui::dialog_detail

namespace ui {

DialogManager::DialogManager(sf::RenderWindow& window)
    : window_(window),
      font_(AssetsManager::get().getFont(ui::font::primaryFamily)) {}

void DialogManager::runModalLoop(Panel& overlay,
                                 const std::function<bool()>& isDone) {
  while (window_.isOpen() && !isDone()) {
    sf::Event event;
    while (window_.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window_.close();
        return;
      }
      overlay.handleEvent(event, window_);
    }
    overlay.update(window_);
    window_.clear(palette::backgroundGrey);
    overlay.render(window_);
    window_.display();
  }
}

std::unique_ptr<Panel> DialogManager::buildModalOverlay(
    sf::Vector2f dialogSize) {
  auto overlay = std::make_unique<Panel>(
      sf::Vector2f{0, 0}, sf::Vector2f{size::width, size::height},
      PanelStyle(palette::black20));

  const float x = (size::width - dialogSize.x) / 2.0f;
  const float y = (size::height - dialogSize.y) / 2.0f;
  auto dialog = std::make_unique<Panel>(
      sf::Vector2f{x, y}, dialogSize,
      PanelStyle(component::panel, palette::black, size::lineThickness));
  overlay->addChild("dialog", std::move(dialog));
  return overlay;
}

void DialogManager::addTitleBand(Panel& panel, const std::string& title,
                                 float width) {
  const LabelStyle labelStyle(
      typography::dialogTitle, palette::white, typography::titleStyle,
      HorizontalAlign::Center, VerticalAlign::Middle,
      {layout::dialog::padding, 0}, false, typography::logBody);

  auto band = std::make_unique<Panel>(
      sf::Vector2f{0, 0}, sf::Vector2f{width, layout::dialog::titleHeight},
      PanelStyle(accent::darkRed));

  auto label = std::make_unique<Label>(
      title, font_, sf::Vector2f{0, 0},
      sf::Vector2f{width, layout::dialog::titleHeight}, labelStyle);

  band->addChild(std::move(label));
  panel.addChild("title-band", std::move(band));
}

void DialogManager::buildDeedPanel(Panel& parent,
                                   const core::Property& property,
                                   sf::Vector2f origin, sf::Vector2f deedSize) {
  const sf::Color band = dialog_detail::propertyBandColor(property);

  auto deed = std::make_unique<Panel>(
      origin, deedSize,
      PanelStyle(palette::white, palette::black, size::lineThickness));

  auto colorBand = std::make_unique<Panel>(
      sf::Vector2f{0, 0},
      sf::Vector2f{deedSize.x, layout::dialog::deedBandHeight},
      PanelStyle(band, palette::black, size::lineThickness));

  const LabelStyle bandLabelStyle(
      typography::dialogTitle, palette::black, typography::titleStyle,
      HorizontalAlign::Center, VerticalAlign::Middle,
      {layout::dialog::padding * 0.5f, 0}, true, typography::logBody);

  auto bandLabel = std::make_unique<Label>(
      property.getName(), font_, sf::Vector2f{0, 0},
      sf::Vector2f{deedSize.x, layout::dialog::deedBandHeight}, bandLabelStyle);

  colorBand->addChild(std::move(bandLabel));
  deed->addChild("band", std::move(colorBand));

  const float priceY =
      layout::dialog::deedBandHeight + layout::dialog::padding * 0.5f;
  const float rowHeight = layout::dialog::buttonHeight * 0.7f;

  const LabelStyle bodyStyle(
      typography::dialogBody, palette::black, typography::regular,
      HorizontalAlign::Left, VerticalAlign::Top,
      {layout::dialog::padding * 0.4f, 0}, false, typography::logBody);

  const LabelStyle bodyRightStyle(
      typography::dialogBody, palette::black, typography::regular,
      HorizontalAlign::Right, VerticalAlign::Top,
      {layout::dialog::padding * 0.4f, 0}, false, typography::logBody);

  auto addRow = [&](float y, const std::string& key, const std::string& value) {
    auto keyLabel = std::make_unique<Label>(
        key, font_, sf::Vector2f{0, y},
        sf::Vector2f{deedSize.x * 0.6f, rowHeight}, bodyStyle);

    auto valueLabel = std::make_unique<Label>(
        value, font_, sf::Vector2f{deedSize.x * 0.6f, y},
        sf::Vector2f{deedSize.x * 0.4f, rowHeight}, bodyRightStyle);

    deed->addChild(std::move(keyLabel));
    deed->addChild(std::move(valueLabel));
  };

  addRow(priceY, "Harga", dialog_detail::formatMoney(property.getPrice()));
  addRow(priceY + rowHeight, "Gadai",
         dialog_detail::formatMoney(property.getMortgageValue()));

  if (property.getType() == core::PropertyTileType::STREET) {
    const auto& street = static_cast<const core::Street&>(property);
    const float baseRent =
        static_cast<float>(property.calculateRent(0, 1, false));

    addRow(priceY + 2 * rowHeight, "Sewa",
           dialog_detail::formatMoney(static_cast<int>(baseRent)));
    addRow(priceY + 3 * rowHeight, "1 Rumah",
           dialog_detail::formatMoney(street.calculateRent(0, 1, true)));
    addRow(priceY + 4 * rowHeight, "2 Rumah",
           dialog_detail::formatMoney(street.calculateRent(0, 1, true)));
    addRow(priceY + 5 * rowHeight, "Rumah Biaya",
           dialog_detail::formatMoney(street.getHouseCost()));
  }

  parent.addChild("deed", std::move(deed));
}

}  // namespace ui
