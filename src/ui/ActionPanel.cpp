#include "ui/ActionPanel.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <algorithm>
#include <memory>
#include <string>

#include "ui/AssetsManager.hpp"
#include "ui/component/Color.hpp"
#include "ui/component/Constants.hpp"
#include "ui/component/Style.hpp"

namespace ui {

ActionPanel::ActionPanel()
    : Panel(
          {0, 0}, {size::sideWidth, size::height},
          PanelStyle(component::sideBar, palette::black, size::lineThickness)),
      font_(AssetsManager::get().getFont(font::primaryFamily)) {
  setup();
}

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
      sf::Vector2f(width, layout::actionPanel::titleHeight), accent::darkRed);
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
      accent::darkGreen, accent::lightGreen, accent::darkGreen,
      component::button::disabled, sf::Color::Transparent,
      layout::actionPanel::borderThickness, buttonLabelStyle);

  const ButtonStyle setDiceStyle(
      accent::darkBlue, accent::blue50, accent::darkBlue,
      component::button::disabled, sf::Color::Transparent,
      layout::actionPanel::borderThickness, buttonLabelStyle);
  const ButtonStyle useCardStyle(
      accent::purple, accent::lavender, accent::purple,
      component::button::disabled, sf::Color::Transparent,
      layout::actionPanel::borderThickness, buttonLabelStyle);

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
  selectedTilePanelWidth_ = width;
  selectedTilePanelY_ = currentY;

  selectedTilePanel_ = std::make_unique<Panel>(
      sf::Vector2f(layout::actionPanel::outerPadding, currentY),
      sf::Vector2f(width, layout::actionPanel::tileInfoHeight),
      accent::darkOrange);

  const LabelStyle tileInfoTitleStyle(
      typography::infoTitle, palette::white, typography::infoTitleStyle,
      HorizontalAlign::Center, VerticalAlign::Top,
      {layout::actionPanel::innerPadding, layout::actionPanel::innerPadding},
      true, typography::logBody);

  auto tileInfoTitle = std::make_unique<Label>(
      "PETAK TERPILIH", font_,
      sf::Vector2f(layout::actionPanel::outerPadding, currentY),
      sf::Vector2f(width, typography::infoTitle +
                              (2.0f * layout::actionPanel::innerPadding)),
      tileInfoTitleStyle);
  selectedTilePanel_->addChild("selected-title", std::move(tileInfoTitle),
                               Panel::Layer::Background);

  setupNoneSelected();

  currentY += layout::actionPanel::tileInfoHeight + layout::actionPanel::rowGap;

  const float tileActionButtonWidth =
      (width - (3.0f * layout::actionPanel::rowGap)) / 4.0f;
  const LabelStyle tileActionLabelStyle(
      typography::buttonSecondary, palette::white, typography::buttonStyle,
      HorizontalAlign::Center, VerticalAlign::Middle,
      {layout::actionPanel::textPaddingX, layout::actionPanel::textPaddingY},
      true, typography::logBody);

  const ButtonStyle buildStyle(
      accent::darkGreen, accent::lightGreen, accent::darkGreen,
      component::button::disabled, sf::Color::Transparent,
      layout::actionPanel::borderThickness, tileActionLabelStyle);
  const ButtonStyle sellStyle(
      accent::red, accent::pink, accent::red, component::button::disabled,
      sf::Color::Transparent, layout::actionPanel::borderThickness,
      tileActionLabelStyle);
  const ButtonStyle mortgageStyle(
      accent::purple, accent::lavender, accent::purple,
      component::button::disabled, sf::Color::Transparent,
      layout::actionPanel::borderThickness, tileActionLabelStyle);
  const ButtonStyle unmortgageStyle(
      accent::darkBlue, accent::blue50, accent::darkBlue,
      component::button::disabled, sf::Color::Transparent,
      layout::actionPanel::borderThickness, tileActionLabelStyle);

  const float x0 = layout::actionPanel::outerPadding;
  const float x1 = x0 + tileActionButtonWidth + layout::actionPanel::rowGap;
  const float x2 = x1 + tileActionButtonWidth + layout::actionPanel::rowGap;
  const float x3 = x2 + tileActionButtonWidth + layout::actionPanel::rowGap;

  buildButton_ = std::make_unique<Button>(
      "BANGUN", font_, sf::Vector2f(x0, currentY),
      sf::Vector2f(tileActionButtonWidth,
                   layout::actionPanel::tileActionHeight),
      buildStyle, []() {
        // TODO: Wire build action for selected tile.
      });

  sellButton_ = std::make_unique<Button>(
      "JUAL", font_, sf::Vector2f(x1, currentY),
      sf::Vector2f(tileActionButtonWidth,
                   layout::actionPanel::tileActionHeight),
      sellStyle, []() {
        // TODO: Wire sell action for selected tile.
      });

  mortgageButton_ = std::make_unique<Button>(
      "SEWA", font_, sf::Vector2f(x2, currentY),
      sf::Vector2f(tileActionButtonWidth,
                   layout::actionPanel::tileActionHeight),
      mortgageStyle, []() {
        // TODO: Wire mortgage action for selected tile.
      });

  unmortgageButton_ = std::make_unique<Button>(
      "GADAI", font_, sf::Vector2f(x3, currentY),
      sf::Vector2f(tileActionButtonWidth,
                   layout::actionPanel::tileActionHeight),
      unmortgageStyle, []() {
        // TODO: Wire unmortgage action for selected tile.
      });

  if (buildButton_) buildButton_->setActive(false);
  if (sellButton_) sellButton_->setActive(false);
  if (mortgageButton_) mortgageButton_->setActive(false);
  if (unmortgageButton_) unmortgageButton_->setActive(false);

  currentY +=
      layout::actionPanel::tileActionHeight + layout::actionPanel::sectionGap;
}

void ActionPanel::setSelectedTile(const TileInfo& tileInfo) {
  setupSelectedExist(tileInfo);
  if (buildButton_) buildButton_->setActive(tileInfo.actionsEnabled);
  if (sellButton_) sellButton_->setActive(tileInfo.actionsEnabled);
  if (mortgageButton_) mortgageButton_->setActive(tileInfo.actionsEnabled);
  if (unmortgageButton_) unmortgageButton_->setActive(tileInfo.actionsEnabled);
}

void ActionPanel::setupNoneSelected() {
  if (!selectedTilePanel_) {
    return;
  }

  selectedTilePanel_->clearChildren(Panel::Layer::Content);

  const float titleHeight =
      typography::infoTitle + (2.0f * layout::actionPanel::innerPadding);
  const float contentY = selectedTilePanelY_ + titleHeight;
  const float contentHeight =
      std::max(0.0f, layout::actionPanel::tileInfoHeight - titleHeight);
  const LabelStyle tileInfoBodyStyle(
      typography::infoBody, palette::white, typography::infoBodyStyle,
      HorizontalAlign::Left, VerticalAlign::Middle,
      {layout::actionPanel::innerPadding, layout::actionPanel::textPaddingY},
      true, typography::logBody);

  auto tileInfoBody = std::make_unique<Label>(
      "Tidak ada petak terpilih", font_,
      sf::Vector2f(layout::actionPanel::outerPadding, contentY),
      sf::Vector2f(selectedTilePanelWidth_, contentHeight), tileInfoBodyStyle);
  selectedTilePanel_->addChild("selected-empty", std::move(tileInfoBody),
                               Panel::Layer::Content);
}

void ActionPanel::setupSelectedExist(const TileInfo& tileInfo) {
  if (!selectedTilePanel_) {
    return;
  }

  selectedTilePanel_->clearChildren(Panel::Layer::Content);

  const float titleHeight =
      typography::infoTitle + (2.0f * layout::actionPanel::innerPadding);
  const float contentY = selectedTilePanelY_ + titleHeight;
  const float contentHeight =
      std::max(0.0f, layout::actionPanel::tileInfoHeight - titleHeight);

  const sf::Vector2f cardPos(
      layout::actionPanel::outerPadding + layout::actionPanel::innerPadding,
      contentY + layout::actionPanel::rowGap);
  const sf::Vector2f cardSize(
      std::max(0.0f, selectedTilePanelWidth_ -
                         (2.0f * layout::actionPanel::innerPadding)),
      std::max(0.0f, contentHeight - (2.0f * layout::actionPanel::rowGap)));

  auto tileCard = std::make_unique<Panel>(cardPos, cardSize, palette::white);
  const float cardInner = layout::actionPanel::innerPadding * 0.75f;
  const float visualSize = std::max(0.0f, cardSize.y - (2.0f * cardInner));
  const float visualWidth = std::min(visualSize, cardSize.x * 0.24f);
  const sf::Vector2f visualPos(cardPos.x + cardInner, cardPos.y + cardInner);
  const sf::Vector2f visualRectSize(visualWidth, visualSize);

  if (tileInfo.useAccent) {
    tileCard->addChild("selected-visual",
                       std::make_unique<Panel>(visualPos, visualRectSize,
                                               tileInfo.accentColor),
                       Panel::Layer::Content);
  } else if (!tileInfo.iconPath.empty()) {
    tileCard->addChild("selected-visual",
                       std::make_unique<ImagePanel>(visualPos, visualRectSize,
                                                    tileInfo.iconPath),
                       Panel::Layer::Content);
  } else {
    tileCard->addChild(
        "selected-visual",
        std::make_unique<Panel>(visualPos, visualRectSize, palette::darkGrey),
        Panel::Layer::Content);
  }

  const float textX =
      visualPos.x + visualRectSize.x + layout::actionPanel::rowGap;
  const float textWidth =
      std::max(0.0f, (cardPos.x + cardSize.x) - textX - cardInner);
  const float nameHeight = std::max(0.0f, cardSize.y);

  const LabelStyle nameStyle(
      typography::infoTitle, palette::black, sf::Text::Bold,
      HorizontalAlign::Left, VerticalAlign::Middle,
      {0.0f, layout::actionPanel::textPaddingY}, true, typography::logBody);

  tileCard->addChild("selected-title-value",
                     std::make_unique<Label>(
                         tileInfo.title, font_, sf::Vector2f(textX, cardPos.y),
                         sf::Vector2f(textWidth, nameHeight), nameStyle),
                     Panel::Layer::Content);

  selectedTilePanel_->addChild("selected-card", std::move(tileCard),
                               Panel::Layer::Content);
}

void ActionPanel::setupLogPanel(float width, float& currentY) {
  auto logHeaderPanel = std::make_unique<Panel>(
      sf::Vector2f(layout::actionPanel::outerPadding, currentY),
      sf::Vector2f(width, layout::actionPanel::logHeaderHeight),
      accent::darkBrown);

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
      accent::brown, palette::black, accent::brown, component::button::disabled,
      palette::black, layout::actionPanel::borderThickness, showMoreLabelStyle);

  auto showMoreButton = std::make_unique<Button>(
      "LIHAT", font_,
      sf::Vector2f(layout::actionPanel::outerPadding + logHeaderLabelWidth,
                   currentY + layout::actionPanel::rowGap),
      sf::Vector2f(layout::actionPanel::headerButtonWidth,
                   layout::actionPanel::logHeaderHeight -
                       (2.0f * layout::actionPanel::rowGap)),
      showMoreStyle, []() {
        // TODO: Open full game log dialog.
      });
  logHeaderPanel->addChild(std::move(showMoreButton));

  addChild(std::move(logHeaderPanel));
  currentY +=
      layout::actionPanel::logHeaderHeight + layout::actionPanel::rowGap;

  auto logBodyPanel = std::make_unique<Panel>(
      sf::Vector2f(layout::actionPanel::outerPadding, currentY),
      sf::Vector2f(width, layout::actionPanel::logBodyHeight), palette::white);

  const LabelStyle logEntryStyle(
      typography::logBody, palette::black, typography::logStyle,
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
      accent::darkGreen, accent::lightGreen, accent::darkGreen,
      component::button::disabled, palette::white,
      layout::actionPanel::borderThickness, optionLabelStyle);
  const ButtonStyle loadStyle(
      accent::darkBlue, accent::blue50, accent::darkBlue,
      component::button::disabled, palette::white,
      layout::actionPanel::borderThickness, optionLabelStyle);
  const ButtonStyle quitStyle(
      accent::red, accent::pink, accent::red, component::button::disabled,
      palette::white, layout::actionPanel::borderThickness, optionLabelStyle);

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
  if (selectedTilePanel_) selectedTilePanel_->handleEvent(event, window);
  if (buildButton_) buildButton_->handleEvent(event, window);
  if (sellButton_) sellButton_->handleEvent(event, window);
  if (mortgageButton_) mortgageButton_->handleEvent(event, window);
  if (unmortgageButton_) unmortgageButton_->handleEvent(event, window);
}

void ActionPanel::render(sf::RenderWindow& window) {
  Panel::render(window);
  if (selectedTilePanel_) selectedTilePanel_->render(window);
  if (buildButton_) buildButton_->render(window);
  if (sellButton_) sellButton_->render(window);
  if (mortgageButton_) mortgageButton_->render(window);
  if (unmortgageButton_) unmortgageButton_->render(window);
}

void ActionPanel::update(sf::RenderWindow& window) {
  Panel::update(window);
  if (selectedTilePanel_) selectedTilePanel_->update(window);
  if (buildButton_) buildButton_->update(window);
  if (sellButton_) sellButton_->update(window);
  if (mortgageButton_) mortgageButton_->update(window);
  if (unmortgageButton_) unmortgageButton_->update(window);
}

}  // namespace ui
