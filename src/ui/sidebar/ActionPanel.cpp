#include "ui/sidebar/ActionPanel.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Mouse.hpp>
#include <algorithm>
#include <cstdlib>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core/Tiles.hpp"
#include "core/card/Card.hpp"
#include "core/player/Player.hpp"
#include "data/LogEntry.hpp"
#include "data/TransactionLogger.hpp"
#include "logic/Game.hpp"
#include "ui/AssetsManager.hpp"
#include "ui/component/Color.hpp"
#include "ui/component/Constants.hpp"
#include "ui/component/Style.hpp"
#include "ui/dialog/DialogManager.hpp"

namespace ui {

namespace {

std::string logActionText(data::LogAction action) {
    switch (action) {
        case data::LogAction::DICE_ROLL:
            return "DADU";
        case data::LogAction::PIECE_MOVEMENT:
            return "GERAK";
        case data::LogAction::PROPERTY_PURCHASE:
            return "BELI";
        case data::LogAction::RENT_PAYMENT:
            return "SEWA";
        case data::LogAction::TAX_PAYMENT:
            return "PAJAK";
        case data::LogAction::BUILD_HOUSE:
            return "BANGUN";
        case data::LogAction::SALE_HOUSE:
            return "JUAL";
        case data::LogAction::MORTGAGE:
            return "GADAI";
        case data::LogAction::UNMORTGAGE:
            return "TEBUS";
        case data::LogAction::SPECIAL_CARD_USE:
            return "KARTU";
        case data::LogAction::CHANCE_CARD_DRAW:
            return "CHANCE";
        case data::LogAction::COMMUNITY_CARD_DRAW:
            return "DANA";
        case data::LogAction::AUCTION_BID:
            return "BID";
        case data::LogAction::AUCTION_RESULT:
            return "LELANG";
        case data::LogAction::FESTIVAL_ACTIVATION:
            return "FEST";
        case data::LogAction::BANKRUPTCY:
            return "BANGKRUT";
        case data::LogAction::GAME_SAVE:
            return "SAVE";
        case data::LogAction::GAME_LOAD:
            return "LOAD";
        default:
            return "LOG";
    }
}

}  // namespace

void ActionPanel::setGameContext(logic::Game* game,
                                                                 DialogManager* dialogManager) {
    game_ = game;
    dialogManager_ = dialogManager;
}

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

    setupStatusRows(width, currentY);

  setupGameActionPanel(width, currentY);
  setupTileOptionPanel(width, currentY);
  setupLogPanel(width, currentY);
  setupGameOptionPanel(width, currentY);
}

void ActionPanel::setupStatusRows(float width, float& currentY) {
    const float rowHeight = layout::actionPanel::topActionHeight * 0.9f;
    const float valueWidth = 1.1f * size::multiplier;
    const float titleWidth = std::max(0.0f, width - valueWidth);

    const LabelStyle leftStyle(
            typography::buttonSecondary, palette::black, typography::titleStyle,
            HorizontalAlign::Left, VerticalAlign::Middle,
            {layout::actionPanel::innerPadding, layout::actionPanel::textPaddingY},
            false, typography::logBody);
    const LabelStyle rightStyle(
            typography::buttonPrimary, palette::black, typography::titleStyle,
            HorizontalAlign::Center, VerticalAlign::Middle,
            {layout::actionPanel::innerPadding, layout::actionPanel::textPaddingY},
            false, typography::logBody);

    auto addRow = [&](const std::string& title, Label** outValue) {
        auto panel = std::make_unique<Panel>(
                sf::Vector2f(layout::actionPanel::outerPadding, currentY),
                sf::Vector2f(width, rowHeight),
                PanelStyle(palette::white, palette::darkGrey,
                                     layout::actionPanel::borderThickness));

        panel->addChild(std::make_unique<Label>(
                title, font_,
                sf::Vector2f(layout::actionPanel::outerPadding, currentY),
                sf::Vector2f(titleWidth, rowHeight), leftStyle));

        auto valueLabel = std::make_unique<Label>(
                "-", font_,
                sf::Vector2f(layout::actionPanel::outerPadding + titleWidth, currentY),
                sf::Vector2f(valueWidth, rowHeight), rightStyle);
        *outValue = valueLabel.get();
        panel->addChild(std::move(valueLabel));

        addChild(std::move(panel));
        currentY += rowHeight + layout::actionPanel::rowGap;
    };

    addRow("TURN", &turnValueLabel_);
    addRow("DADU", &diceValueLabel_);

    currentY += layout::actionPanel::sectionGap - layout::actionPanel::rowGap;
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

    addTopButton("Lanjutkan | Lempar Dadu", nextTurnStyle, [this]() {
        if (game_ == nullptr) return;
        try {
            game_->rollDice();
            game_->moveCurrentPlayer();
        } catch (...) {
        }
  });

    addTopButton("Lanjutkan | Atur Dadu", setDiceStyle, [this]() {
        if (game_ == nullptr || dialogManager_ == nullptr) return;
        const auto [d1, d2] = dialogManager_->promptDiceOverride();
        try {
            game_->setDice(d1, d2);
            game_->moveCurrentPlayer();
        } catch (...) {
        }
  });

    addTopButton("Gunakan Kartu", useCardStyle, [this]() {
        if (game_ == nullptr || dialogManager_ == nullptr) return;
        core::Player* current = game_->getCurrentPlayer();
        if (current == nullptr) return;

        auto hand = current->getHeldCards();
        if (hand.empty()) return;

        int chosenIdx = dialogManager_->promptCardChoice(*current);
        chosenIdx =
                std::max(0, std::min(chosenIdx, static_cast<int>(hand.size()) - 1));
        core::ActionCard* chosenCard = hand[static_cast<size_t>(chosenIdx)];
        auto owned = current->removeCard(chosenCard);
        if (owned) {
            owned->execute(*current, *game_);
        }
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
            buildStyle, [this]() {
                if (game_ == nullptr) return;
                core::Player* current = game_->getCurrentPlayer();
                if (current == nullptr) return;
                core::Tile* tile = game_->getBoard().getTile(current->getPosition());
                if (tile == nullptr) return;
                try {
                    game_->buildHouse(current, tile);
                } catch (...) {
                }
      });

  sellButton_ = std::make_unique<Button>(
      "JUAL", font_, sf::Vector2f(x1, currentY),
      sf::Vector2f(tileActionButtonWidth,
                   layout::actionPanel::tileActionHeight),
            sellStyle, [this]() {
                if (game_ == nullptr) return;
                core::Player* current = game_->getCurrentPlayer();
                if (current == nullptr) return;
                core::Tile* tile = game_->getBoard().getTile(current->getPosition());
                if (tile == nullptr) return;
                try {
                    game_->sellHouse(current, tile);
                } catch (...) {
                }
      });

  mortgageButton_ = std::make_unique<Button>(
      "GADAI", font_, sf::Vector2f(x2, currentY),
      sf::Vector2f(tileActionButtonWidth,
                   layout::actionPanel::tileActionHeight),
            mortgageStyle, [this]() {
                if (game_ == nullptr) return;
                core::Player* current = game_->getCurrentPlayer();
                if (current == nullptr) return;
                core::Tile* tile = game_->getBoard().getTile(current->getPosition());
                auto* propertyTile = dynamic_cast<core::PropertyTile*>(tile);
                if (propertyTile == nullptr) return;
                try {
                    game_->mortgageProperty(propertyTile->getProperty());
                } catch (...) {
                }
      });

  unmortgageButton_ = std::make_unique<Button>(
      "TEBUS", font_, sf::Vector2f(x3, currentY),
      sf::Vector2f(tileActionButtonWidth,
                   layout::actionPanel::tileActionHeight),
            unmortgageStyle, [this]() {
                if (game_ == nullptr) return;
                core::Player* current = game_->getCurrentPlayer();
                if (current == nullptr) return;
                core::Tile* tile = game_->getBoard().getTile(current->getPosition());
                auto* propertyTile = dynamic_cast<core::PropertyTile*>(tile);
                if (propertyTile == nullptr) return;
                try {
                    game_->unmortgageProperty(propertyTile->getProperty());
                } catch (...) {
                }
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
            showMoreStyle, [this]() {
                logFirstVisibleLine_ = 0;
      });
  logHeaderPanel->addChild(std::move(showMoreButton));

  addChild(std::move(logHeaderPanel));
  currentY +=
      layout::actionPanel::logHeaderHeight + layout::actionPanel::rowGap;

  auto logBodyPanel = std::make_unique<Panel>(
      sf::Vector2f(layout::actionPanel::outerPadding, currentY),
      sf::Vector2f(width, layout::actionPanel::logBodyHeight), palette::white);

  logBodyRect_ = sf::FloatRect(layout::actionPanel::outerPadding, currentY,
                               width, layout::actionPanel::logBodyHeight);
  logLineHeight_ = static_cast<float>(typography::logBody) +
                   layout::actionPanel::textPaddingY * 0.9f;

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
                data::TransactionLogger::get().serialize("data/test_log.txt");
      }));

  addChild(std::make_unique<Button>(
      "MUAT", font_,
      sf::Vector2f(layout::actionPanel::outerPadding + gameOptionWidth +
                       layout::actionPanel::rowGap,
                   currentY),
      sf::Vector2f(gameOptionWidth, layout::actionPanel::gameOptionHeight),
            loadStyle, []() {
                data::TransactionLogger::get().deserialize("data/test_log.txt");
      }));

  addChild(std::make_unique<Button>(
      "KELUAR", font_,
      sf::Vector2f(layout::actionPanel::outerPadding +
                       (2.0f * (gameOptionWidth + layout::actionPanel::rowGap)),
                   currentY),
      sf::Vector2f(gameOptionWidth, layout::actionPanel::gameOptionHeight),
            quitStyle, []() {
                std::exit(0);
      }));
}

void ActionPanel::refreshStatusRows() {
    if (game_ == nullptr) {
        if (turnValueLabel_) turnValueLabel_->setText("-");
        if (diceValueLabel_) diceValueLabel_->setText("-");
        return;
    }

    if (turnValueLabel_) {
        turnValueLabel_->setText(std::to_string(game_->getTurnCount()));
    }

    if (diceValueLabel_) {
        const auto [d1, d2] = game_->getLastDiceRoll();
        if (d1 <= 0 || d2 <= 0) {
            diceValueLabel_->setText("-");
        } else {
            diceValueLabel_->setText(std::to_string(d1) + " + " +
                                                             std::to_string(d2));
        }
    }
}

void ActionPanel::refreshLogLines() {
    const auto& entries = data::TransactionLogger::get().getEntries();
    logLines_.clear();
    logLines_.reserve(entries.size());

    for (auto it = entries.rbegin(); it != entries.rend(); ++it) {
        const data::LogEntry& e = *it;
        logLines_.push_back("T" + std::to_string(e.getTurnNumber()) + " [" +
                                                logActionText(e.getActionType()) + "] " +
                                                e.getPlayerName() + " - " + e.getDescription());
    }

    if (logLines_.empty()) {
        logLines_.push_back("Belum ada transaksi.");
    }

    const float innerHeight =
            std::max(0.0f, logBodyRect_.height - 2.0f * layout::actionPanel::innerPadding);
    logVisibleLineCount_ =
            std::max(1, static_cast<int>(innerHeight / std::max(1.0f, logLineHeight_)));
    clampLogScroll();

    const float trackWidth = layout::actionPanel::innerPadding * 0.75f;
    logTrackRect_ = sf::FloatRect(
            logBodyRect_.left + logBodyRect_.width - trackWidth -
                    layout::actionPanel::innerPadding * 0.35f,
            logBodyRect_.top + layout::actionPanel::innerPadding * 0.35f,
            trackWidth,
            logBodyRect_.height - layout::actionPanel::innerPadding * 0.7f);

    const int total = static_cast<int>(logLines_.size());
    const int maxFirst = std::max(0, total - logVisibleLineCount_);
    const float ratio = std::min(1.0f,
                                                             static_cast<float>(logVisibleLineCount_) /
                                                                     static_cast<float>(std::max(1, total)));
    const float thumbHeight = std::max(18.0f, logTrackRect_.height * ratio);
    const float travel = std::max(0.0f, logTrackRect_.height - thumbHeight);
    const float t = maxFirst == 0
                                            ? 0.0f
                                            : static_cast<float>(logFirstVisibleLine_) /
                                                        static_cast<float>(maxFirst);

    logThumbRect_ = sf::FloatRect(logTrackRect_.left,
                                                                logTrackRect_.top + travel * t,
                                                                logTrackRect_.width,
                                                                thumbHeight);
}

void ActionPanel::clampLogScroll() {
    const int maxFirst =
            std::max(0, static_cast<int>(logLines_.size()) - logVisibleLineCount_);
    logFirstVisibleLine_ = std::max(0, std::min(logFirstVisibleLine_, maxFirst));
}

bool ActionPanel::isInRect(const sf::Vector2f& point,
                                                     const sf::FloatRect& rect) const {
    return rect.contains(point);
}

void ActionPanel::handleEvent(sf::Event& event, sf::RenderWindow& window) {
  Panel::handleEvent(event, window);
  if (selectedTilePanel_) selectedTilePanel_->handleEvent(event, window);
  if (buildButton_) buildButton_->handleEvent(event, window);
  if (sellButton_) sellButton_->handleEvent(event, window);
  if (mortgageButton_) mortgageButton_->handleEvent(event, window);
  if (unmortgageButton_) unmortgageButton_->handleEvent(event, window);

    const sf::Vector2f mousePos =
            window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (event.type == sf::Event::MouseWheelScrolled &&
            isInRect(mousePos, logBodyRect_)) {
        if (event.mouseWheelScroll.delta > 0.0f) {
            logFirstVisibleLine_ = std::max(0, logFirstVisibleLine_ - 2);
        } else if (event.mouseWheelScroll.delta < 0.0f) {
            logFirstVisibleLine_ += 2;
            clampLogScroll();
        }
    }

    if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left &&
            isInRect(mousePos, logThumbRect_)) {
        draggingLogThumb_ = true;
        logDragOffsetY_ = mousePos.y - logThumbRect_.top;
    }

    if (event.type == sf::Event::MouseButtonReleased &&
            event.mouseButton.button == sf::Mouse::Left) {
        draggingLogThumb_ = false;
    }

    if (event.type == sf::Event::MouseMoved && draggingLogThumb_) {
        const float minY = logTrackRect_.top;
        const float maxY = logTrackRect_.top + logTrackRect_.height - logThumbRect_.height;
        const float rawTop = mousePos.y - logDragOffsetY_;
        const float thumbTop = std::max(minY, std::min(rawTop, maxY));
        const float travel = std::max(1.0f, maxY - minY);
        const float t = (thumbTop - minY) / travel;

        const int maxFirst =
                std::max(0, static_cast<int>(logLines_.size()) - logVisibleLineCount_);
        logFirstVisibleLine_ = static_cast<int>(t * static_cast<float>(maxFirst));
        clampLogScroll();
    }
}

void ActionPanel::render(sf::RenderWindow& window) {
  Panel::render(window);
  if (selectedTilePanel_) selectedTilePanel_->render(window);
  if (buildButton_) buildButton_->render(window);
  if (sellButton_) sellButton_->render(window);
  if (mortgageButton_) mortgageButton_->render(window);
  if (unmortgageButton_) unmortgageButton_->render(window);

    const float textStartX = logBodyRect_.left + layout::actionPanel::innerPadding;
    const float textStartY = logBodyRect_.top + layout::actionPanel::innerPadding;
    const float usableWidth =
            std::max(0.0f, logBodyRect_.width - logTrackRect_.width -
                                                 2.4f * layout::actionPanel::innerPadding);

    const LabelStyle lineStyle(
            typography::logBody, palette::black, typography::logStyle,
            HorizontalAlign::Left, VerticalAlign::Top, {0.0f, 0.0f}, true,
            typography::logBody);

    for (int i = 0; i < logVisibleLineCount_; ++i) {
        const int index = logFirstVisibleLine_ + i;
        if (index < 0 || index >= static_cast<int>(logLines_.size())) {
            break;
        }
        Label line(logLines_[static_cast<size_t>(index)], font_,
                             sf::Vector2f(textStartX, textStartY + i * logLineHeight_),
                             sf::Vector2f(usableWidth, logLineHeight_), lineStyle);
        line.render(window);
    }

    if (logLines_.size() > static_cast<size_t>(logVisibleLineCount_)) {
        sf::RectangleShape track;
        track.setPosition({logTrackRect_.left, logTrackRect_.top});
        track.setSize({logTrackRect_.width, logTrackRect_.height});
        track.setFillColor(palette::lightGrey);
        track.setOutlineColor(palette::darkGrey);
        track.setOutlineThickness(1.0f);
        window.draw(track);

        sf::RectangleShape thumb;
        thumb.setPosition({logThumbRect_.left, logThumbRect_.top});
        thumb.setSize({logThumbRect_.width, logThumbRect_.height});
        thumb.setFillColor(accent::darkBlue);
        window.draw(thumb);
    }
}

void ActionPanel::update(sf::RenderWindow& window) {
  Panel::update(window);
  if (selectedTilePanel_) selectedTilePanel_->update(window);
  if (buildButton_) buildButton_->update(window);
  if (sellButton_) sellButton_->update(window);
  if (mortgageButton_) mortgageButton_->update(window);
  if (unmortgageButton_) unmortgageButton_->update(window);

    refreshStatusRows();
    refreshLogLines();
}

}  // namespace ui
