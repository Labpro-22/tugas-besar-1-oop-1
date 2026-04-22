#include "ui/ActionPanel.hpp"

#include <memory>
#include <stdexcept>
#include <string>

#include "ui/Color.hpp"
#include "ui/Constants.hpp"

namespace ui {

ActionPanel::ActionPanel()
    : Panel({0, 0}, {size::sideWidth, size::height}, component::sideBar) {
  if (!font_.loadFromFile(font::primaryFamily)) {
    throw std::runtime_error(
        std::string("Unable to load font from ").append(font::primaryFamily));
  }
  setup();
}

ActionPanel::~ActionPanel() = default;

void ActionPanel::setup() {
  const float width =
      size::sideWidth - (2.0f * layout::actionPanel::outerPadding);
  float currentY = layout::actionPanel::outerPadding;

  const LabelStyle titleStyle(
      typography::title, palette::white, typography::titleStyle,
      HorizontalAlign::Center, VerticalAlign::Middle,
      {layout::actionPanel::textPaddingX, layout::actionPanel::textPaddingY},
      false, typography::logBody);

  auto titlePanel = std::make_unique<Panel>(
      sf::Vector2f(layout::actionPanel::outerPadding, currentY),
      sf::Vector2f(width, layout::actionPanel::titleHeight), component::card);
  auto titleLabel = std::make_unique<Label>(
      "Menu", font_, sf::Vector2f(layout::actionPanel::outerPadding, currentY),
      sf::Vector2f(width, layout::actionPanel::titleHeight), titleStyle);
  titlePanel->addChild(std::move(titleLabel));
  addChild(std::move(titlePanel));

  currentY +=
      layout::actionPanel::titleHeight + layout::actionPanel::sectionGap;

  setupGameActionPanel(width, currentY);
  setupTileOptionPanel(width, currentY);
  setupLogPanel(width, currentY);
  setupGameOptionPanel(width, currentY);
}

void ActionPanel::setupGameActionPanel(float width, float& currentY) {
  const LabelStyle buttonLabelStyle(
      typography::buttonPrimary, palette::white, typography::buttonStyle,
      HorizontalAlign::Center, VerticalAlign::Middle,
      {layout::actionPanel::textPaddingX, layout::actionPanel::textPaddingY},
      true, typography::logBody);

  const ButtonStyle nextTurnStyle(
      component::button::green, accent::lime, accent::green,
      component::button::disabled, component::border,
      layout::actionPanel::borderThickness, buttonLabelStyle);
  const ButtonStyle setDiceStyle(
      accent::darkBlue, component::button::blue, component::button::blue,
      component::button::disabled, component::border,
      layout::actionPanel::borderThickness, buttonLabelStyle);
  const ButtonStyle useCardStyle(accent::orange, accent::yellow, accent::orange,
                                 component::button::disabled, component::border,
                                 layout::actionPanel::borderThickness,
                                 buttonLabelStyle);

  auto addTopButton = [&](const std::string& text, const ButtonStyle& style,
                          std::function<void()> clickHandler) {
    addChild(std::make_unique<Button>(
        text, font_, sf::Vector2f(layout::actionPanel::outerPadding, currentY),
        sf::Vector2f(width, layout::actionPanel::topActionHeight), style,
        std::move(clickHandler)));
    currentY +=
        layout::actionPanel::topActionHeight + layout::actionPanel::rowGap;
  };

  addTopButton("Lanjutkan | Lempar Dadu", nextTurnStyle, []() {
    // TODO: Wire roll-dice action to game turn manager.
  });
  addTopButton("Lanjutkan | Atur Dadu", setDiceStyle, []() {
    // TODO: Open manual dice input dialog.
  });
  addTopButton("Gunakan Kartu", useCardStyle, []() {
    // TODO: Open ability card selection dialog.
  });

  currentY += layout::actionPanel::sectionGap - layout::actionPanel::rowGap;
}

void ActionPanel::setupTileOptionPanel(float width, float& currentY) {
  auto selectedTilePanel = std::make_unique<Panel>(
      sf::Vector2f(layout::actionPanel::outerPadding, currentY),
      sf::Vector2f(width, layout::actionPanel::tileInfoHeight),
      component::card);

  const LabelStyle tileInfoTitleStyle(
      typography::infoTitle, palette::white, typography::infoTitleStyle,
      HorizontalAlign::Left, VerticalAlign::Top,
      {layout::actionPanel::innerPadding, layout::actionPanel::innerPadding},
      true, typography::logBody);
  const LabelStyle tileInfoBodyStyle(
      typography::infoBody, palette::lightGrey, typography::infoBodyStyle,
      HorizontalAlign::Left, VerticalAlign::Middle,
      {layout::actionPanel::innerPadding, layout::actionPanel::textPaddingY},
      true, typography::logBody);

  auto tileInfoTitle = std::make_unique<Label>(
      "PETAK TERPILIH", font_,
      sf::Vector2f(layout::actionPanel::outerPadding, currentY),
      sf::Vector2f(width, typography::infoTitle +
                              (2.0f * layout::actionPanel::innerPadding)),
      tileInfoTitleStyle);
  selectedTilePanel->addChild(std::move(tileInfoTitle));

  auto tileInfoBody = std::make_unique<Label>(
      "Tidak ada petak terpilih", font_,
      sf::Vector2f(layout::actionPanel::outerPadding, currentY),
      sf::Vector2f(width, layout::actionPanel::tileInfoHeight),
      tileInfoBodyStyle);
  selectedTilePanel->addChild(std::move(tileInfoBody));

  addChild(std::move(selectedTilePanel));
  currentY += layout::actionPanel::tileInfoHeight + layout::actionPanel::rowGap;

  const float tileActionButtonWidth =
      (width - (3.0f * layout::actionPanel::rowGap)) / 4.0f;
  const LabelStyle tileActionLabelStyle(
      typography::buttonSecondary, palette::white, typography::buttonStyle,
      HorizontalAlign::Center, VerticalAlign::Middle,
      {layout::actionPanel::textPaddingX, layout::actionPanel::textPaddingY},
      true, typography::logBody);

  const ButtonStyle buildStyle(
      component::button::green, accent::lime, accent::green,
      component::button::disabled, component::border,
      layout::actionPanel::borderThickness, tileActionLabelStyle);
  const ButtonStyle sellStyle(
      component::button::redAction, accent::pink, accent::magenta,
      component::button::disabled, component::border,
      layout::actionPanel::borderThickness, tileActionLabelStyle);
  const ButtonStyle mortgageStyle(
      component::button::purple, accent::magenta, accent::lavender,
      component::button::disabled, component::border,
      layout::actionPanel::borderThickness, tileActionLabelStyle);
  const ButtonStyle unmortgageStyle(
      accent::darkBlue, component::button::blue, component::button::blue,
      component::button::disabled, component::border,
      layout::actionPanel::borderThickness, tileActionLabelStyle);

  const float x0 = layout::actionPanel::outerPadding;
  const float x1 = x0 + tileActionButtonWidth + layout::actionPanel::rowGap;
  const float x2 = x1 + tileActionButtonWidth + layout::actionPanel::rowGap;
  const float x3 = x2 + tileActionButtonWidth + layout::actionPanel::rowGap;

  addChild(std::make_unique<Button>(
      "BANGUN", font_, sf::Vector2f(x0, currentY),
      sf::Vector2f(tileActionButtonWidth,
                   layout::actionPanel::tileActionHeight),
      buildStyle, []() {
        // TODO: Wire build action for selected tile.
      }));
  addChild(std::make_unique<Button>(
      "JUAL", font_, sf::Vector2f(x1, currentY),
      sf::Vector2f(tileActionButtonWidth,
                   layout::actionPanel::tileActionHeight),
      sellStyle, []() {
        // TODO: Wire sell action for selected tile.
      }));
  addChild(std::make_unique<Button>(
      "SEWA", font_, sf::Vector2f(x2, currentY),
      sf::Vector2f(tileActionButtonWidth,
                   layout::actionPanel::tileActionHeight),
      mortgageStyle, []() {
        // TODO: Wire mortgage action for selected tile.
      }));
  addChild(std::make_unique<Button>(
      "GADAI", font_, sf::Vector2f(x3, currentY),
      sf::Vector2f(tileActionButtonWidth,
                   layout::actionPanel::tileActionHeight),
      unmortgageStyle, []() {
        // TODO: Wire unmortgage action for selected tile.
      }));

  currentY +=
      layout::actionPanel::tileActionHeight + layout::actionPanel::sectionGap;
}

void ActionPanel::setupLogPanel(float width, float& currentY) {
  auto logHeaderPanel = std::make_unique<Panel>(
      sf::Vector2f(layout::actionPanel::outerPadding, currentY),
      sf::Vector2f(width, layout::actionPanel::logHeaderHeight),
      component::card);

  const float logHeaderLabelWidth = width -
                                    layout::actionPanel::headerButtonWidth -
                                    layout::actionPanel::rowGap;
  const LabelStyle logHeaderStyle(
      typography::logTitle, palette::white, typography::sectionStyle,
      HorizontalAlign::Left, VerticalAlign::Middle,
      {layout::actionPanel::innerPadding, layout::actionPanel::textPaddingY},
      true, typography::logBody);

  auto logTitle = std::make_unique<Label>(
      "LOG TRANSAKSI", font_,
      sf::Vector2f(layout::actionPanel::outerPadding, currentY),
      sf::Vector2f(logHeaderLabelWidth, layout::actionPanel::logHeaderHeight),
      logHeaderStyle);
  logHeaderPanel->addChild(std::move(logTitle));

  const LabelStyle showMoreLabelStyle(
      typography::buttonSecondary, palette::white, typography::buttonStyle,
      HorizontalAlign::Center, VerticalAlign::Middle,
      {layout::actionPanel::textPaddingX, layout::actionPanel::textPaddingY},
      true, typography::logBody);
  const ButtonStyle showMoreStyle(
      component::button::orange, accent::yellow, accent::orange,
      component::button::disabled, component::border,
      layout::actionPanel::borderThickness, showMoreLabelStyle);

  auto showMoreButton = std::make_unique<Button>(
      "LIHAT", font_,
      sf::Vector2f(layout::actionPanel::outerPadding + logHeaderLabelWidth +
                       layout::actionPanel::rowGap,
                   currentY),
      sf::Vector2f(layout::actionPanel::headerButtonWidth,
                   layout::actionPanel::logHeaderHeight),
      showMoreStyle, []() {
        // TODO: Open full game log dialog.
      });
  logHeaderPanel->addChild(std::move(showMoreButton));

  addChild(std::move(logHeaderPanel));
  currentY +=
      layout::actionPanel::logHeaderHeight + layout::actionPanel::rowGap;

  auto logBodyPanel = std::make_unique<Panel>(
      sf::Vector2f(layout::actionPanel::outerPadding, currentY),
      sf::Vector2f(width, layout::actionPanel::logBodyHeight),
      palette::lightGrey);

  const LabelStyle logEntryStyle(
      typography::logBody, component::border, typography::logStyle,
      HorizontalAlign::Left, VerticalAlign::Top,
      {layout::actionPanel::innerPadding, layout::actionPanel::innerPadding},
      true, typography::logBody);

  auto logEntryOne = std::make_unique<Label>(
      "Belum ada transaksi", font_,
      sf::Vector2f(layout::actionPanel::outerPadding, currentY),
      sf::Vector2f(width, layout::actionPanel::logBodyHeight / 3.0f),
      logEntryStyle);
  logBodyPanel->addChild(std::move(logEntryOne));

  auto logEntryTwo = std::make_unique<Label>(
      "Transaksi akan terlihat di sini.", font_,
      sf::Vector2f(layout::actionPanel::outerPadding,
                   currentY + (layout::actionPanel::logBodyHeight / 3.0f)),
      sf::Vector2f(width, layout::actionPanel::logBodyHeight / 3.0f),
      logEntryStyle);
  logBodyPanel->addChild(std::move(logEntryTwo));

  addChild(std::move(logBodyPanel));
  currentY +=
      layout::actionPanel::logBodyHeight + layout::actionPanel::sectionGap;
}

void ActionPanel::setupGameOptionPanel(float width, float& currentY) {
  const float gameOptionWidth =
      (width - (2.0f * layout::actionPanel::rowGap)) / 3.0f;
  const LabelStyle optionLabelStyle(
      typography::buttonSecondary, palette::white, typography::buttonStyle,
      HorizontalAlign::Center, VerticalAlign::Middle,
      {layout::actionPanel::textPaddingX, layout::actionPanel::textPaddingY},
      true, typography::logBody);
  const ButtonStyle saveStyle(
      component::button::green, accent::lime, accent::green,
      component::button::disabled, component::border,
      layout::actionPanel::borderThickness, optionLabelStyle);
  const ButtonStyle loadStyle(
      accent::darkBlue, component::button::blue, component::button::blue,
      component::button::disabled, component::border,
      layout::actionPanel::borderThickness, optionLabelStyle);
  const ButtonStyle quitStyle(
      component::button::redAction, accent::pink, accent::magenta,
      component::button::disabled, component::border,
      layout::actionPanel::borderThickness, optionLabelStyle);

  addChild(std::make_unique<Button>(
      "SIMPAN", font_,
      sf::Vector2f(layout::actionPanel::outerPadding, currentY),
      sf::Vector2f(gameOptionWidth, layout::actionPanel::gameOptionHeight),
      saveStyle, []() {
        // TODO: Open save game dialog.
      }));

  addChild(std::make_unique<Button>(
      "MUAT", font_,
      sf::Vector2f(layout::actionPanel::outerPadding + gameOptionWidth +
                       layout::actionPanel::rowGap,
                   currentY),
      sf::Vector2f(gameOptionWidth, layout::actionPanel::gameOptionHeight),
      loadStyle, []() {
        // TODO: Open load game dialog.
      }));

  addChild(std::make_unique<Button>(
      "KELUAR", font_,
      sf::Vector2f(layout::actionPanel::outerPadding +
                       (2.0f * (gameOptionWidth + layout::actionPanel::rowGap)),
                   currentY),
      sf::Vector2f(gameOptionWidth, layout::actionPanel::gameOptionHeight),
      quitStyle, []() {
        // TODO: Open quit confirmation dialog.
      }));
}

void ActionPanel::handleEvent(sf::Event& event, sf::RenderWindow& window) {
  Panel::handleEvent(event, window);
}

void ActionPanel::render(sf::RenderWindow& window) { Panel::render(window); }

void ActionPanel::update(sf::RenderWindow& window) { Panel::update(window); }

}  // namespace ui
