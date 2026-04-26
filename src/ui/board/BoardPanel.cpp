#include "ui/board/BoardPanel.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "core/Tiles.hpp"
#include "core/player/Player.hpp"
#include "logic/Board.hpp"
#include "ui/AssetsManager.hpp"
#include "ui/board/ActionTilePanel.hpp"
#include "ui/board/CornerTilePanel.hpp"
#include "ui/board/StreetTilePanel.hpp"
#include "ui/board/TileGeometry.hpp"
#include "ui/component/Color.hpp"
#include "ui/component/Constants.hpp"
#include "ui/component/PropertyView.hpp"
#include "ui/component/Widgets.hpp"

namespace {

std::string asMoney(int amount) { return "$ " + std::to_string(amount); }

std::string avatarPathFor(core::Avatar avatar) {
  switch (avatar) {
    case core::Avatar::COPILOT:
      return "assets/players/copilot.png";
    case core::Avatar::CLAUDE:
      return "assets/players/claude.png";
    case core::Avatar::GEMINI:
      return "assets/players/gemini.png";
    case core::Avatar::CHATGPT:
      return "assets/players/openai.png";
    default:
      return "assets/players/copilot.png";
  }
}

struct TilePlacement {
  sf::Vector2f position;
  ui::Orientation orientation;
  bool isCorner;
};

TilePlacement placementForIndex(const sf::Vector2f& boardPos,
                                const sf::Vector2f& boardSize, int tileNum,
                                int index) {
  const ui::TileGeometry geometry(tileNum);
  const float corner = geometry.cornerSide();
  const float tileW = geometry.sideWidth();
  const int span = tileNum + 1;

  if (index == 0) {
    return {{boardPos.x + boardSize.x - corner, boardPos.y + boardSize.y - corner},
            ui::Orientation::Bottom, true};
  }

  if (index < span) {
    const int step = index - 1;
    return {{boardPos.x + boardSize.x - corner - tileW - step * tileW,
             boardPos.y + boardSize.y - corner},
            ui::Orientation::Bottom, false};
  }

  if (index == span) {
    return {{boardPos.x, boardPos.y + boardSize.y - corner},
            ui::Orientation::Left, true};
  }

  if (index < 2 * span) {
    const int step = index - span - 1;
    return {{boardPos.x,
             boardPos.y + boardSize.y - corner - tileW - step * tileW},
            ui::Orientation::Left, false};
  }

  if (index == 2 * span) {
    return {{boardPos.x, boardPos.y}, ui::Orientation::Top, true};
  }

  if (index < 3 * span) {
    const int step = index - 2 * span - 1;
    return {{boardPos.x + corner + step * tileW, boardPos.y},
            ui::Orientation::Top, false};
  }

  if (index == 3 * span) {
    return {{boardPos.x + boardSize.x - corner, boardPos.y},
            ui::Orientation::Right, true};
  }

  const int step = index - 3 * span - 1;
  return {{boardPos.x + boardSize.x - corner, boardPos.y + corner + step * tileW},
          ui::Orientation::Right, false};
}

std::unique_ptr<ui::TilePanel> buildPropertyPanel(const core::PropertyTile& tile,
                                                  const TilePlacement& place,
                                                  int tileNum) {
  const core::Property& property = tile.getProperty();
  const ui::PropertyView view = ui::buildPropertyView(property);

  if (tile.getPropertyTileType() == core::PropertyTileType::STREET) {
    return std::make_unique<ui::StreetTilePanel>(
        place.position, tileNum, place.orientation, view.groupColor, view.code,
        asMoney(property.getPrice()));
  }

  std::string iconPath = "assets/icons/train.png";
  if (tile.getPropertyTileType() == core::PropertyTileType::UTILITY) {
    iconPath = view.utilityIconPath.empty() ? "assets/icons/electricity.png"
                                            : view.utilityIconPath;
  } else if (tile.getPropertyTileType() == core::PropertyTileType::RAILROAD) {
    iconPath = "assets/icons/train-kai.png";
  }

  return std::make_unique<ui::ActionTilePanel>(
      place.position, tileNum, place.orientation, iconPath, view.code,
      asMoney(property.getPrice()));
}

std::unique_ptr<ui::TilePanel> buildActionPanel(const core::ActionTile& tile,
                                                const TilePlacement& place,
                                                int tileNum) {
  if (place.isCorner) {
    core::SpecialTileType specialType = core::SpecialTileType::GO;
    std::string icon = "assets/icons/go.png";
    std::string top = "PETAK";
    std::string bottom = "UJUNG";

    const auto* special = dynamic_cast<const core::SpecialTile*>(&tile);
    if (special != nullptr) {
      specialType = special->getSpecialTileType();
      switch (specialType) {
        case core::SpecialTileType::GO:
          icon = "assets/icons/go.png";
          top = "MULAI";
          bottom = "DAPAT UANG";
          break;
        case core::SpecialTileType::JAIL:
          icon = "assets/icons/jail.png";
          top = "HANYA";
          bottom = "BERKUNJUNG";
          break;
        case core::SpecialTileType::FREE_PARKING:
          icon = "assets/icons/free-park.png";
          top = "PARKIR";
          bottom = "GRATIS";
          break;
        case core::SpecialTileType::GO_TO_JAIL:
          icon = "assets/icons/go-to-jail.png";
          top = "PERGI";
          bottom = "KE PENJARA";
          break;
      }
    }

    return std::make_unique<ui::CornerTilePanel>(
        place.position, tileNum, specialType, place.orientation, icon, top,
        bottom);
  }

  std::string icon = "assets/icons/chance-pink.png";
  std::string top = "AKSI";
  std::string bottom = "";

  if (const auto* card = dynamic_cast<const core::CardTile*>(&tile)) {
    if (card->isChance()) {
      icon = "assets/icons/chance-pink.png";
      top = "KESEMPATAN";
    } else {
      icon = "assets/icons/community.png";
      top = "DANA UMUM";
    }
  } else if (const auto* tax = dynamic_cast<const core::TaxTile*>(&tile)) {
    icon = tax->getTaxType() == core::TaxType::PPH
               ? "assets/icons/income-tax.png"
               : "assets/icons/luxury-tax.png";
    top = "PAJAK";
    bottom = asMoney(tax->getFlatRate());
  } else if (dynamic_cast<const core::FestivalTile*>(&tile) != nullptr) {
    icon = "assets/icons/festival.png";
    top = "FESTIVAL";
  } else if (dynamic_cast<const core::SpecialTile*>(&tile) != nullptr) {
    icon = "assets/icons/go.png";
    top = "SPESIAL";
  }

  return std::make_unique<ui::ActionTilePanel>(
      place.position, tileNum, place.orientation, icon, top, bottom);
}

}  // namespace

namespace ui {

BoardPanel::BoardPanel(int tileNum)
    : Panel({size::sideWidth, 0}, {size::boardSize, size::boardSize},
            PanelStyle(component::background, palette::black,
                       size::lineThickness)),
      tileNum_(tileNum) {
  if (tileNum_ < minTilesPerSide || tileNum_ > maxTilesPerSide) {
    throw std::runtime_error("Invalid board configuration.");
  }

  auto backgroundPanel = std::make_unique<Panel>(position(), size(), board::base);
  addChild("board-bg", std::move(backgroundPanel));

  avatarTextures_[core::Avatar::COPILOT] =
      &AssetsManager::get().getTexture(avatarPathFor(core::Avatar::COPILOT));
  avatarTextures_[core::Avatar::CLAUDE] =
      &AssetsManager::get().getTexture(avatarPathFor(core::Avatar::CLAUDE));
  avatarTextures_[core::Avatar::GEMINI] =
      &AssetsManager::get().getTexture(avatarPathFor(core::Avatar::GEMINI));
  avatarTextures_[core::Avatar::CHATGPT] =
      &AssetsManager::get().getTexture(avatarPathFor(core::Avatar::CHATGPT));

  setup();
}

void BoardPanel::setTileCount(int tileNum) {
  tileNum_ = std::max(minTilesPerSide, std::min(maxTilesPerSide, tileNum));
}

void BoardPanel::setPlayers(const std::vector<core::Player*>& players) {
  players_ = players;
}

void BoardPanel::setOnTileSelected(
    std::function<void(const TileInfo&)> onTileSelected) {
  onTileSelected_ = std::move(onTileSelected);

  for (auto& entry : children_) {
    auto* tile = dynamic_cast<TilePanel*>(std::get<2>(entry).get());
    if (tile) {
      tile->setOnSelected(onTileSelected_);
    }
  }
}

void BoardPanel::setup() {
  // Populated through populateTiles(board) once game initialization is ready.
}

void BoardPanel::populateTiles(const logic::Board& gameBoard) {
  clearChildren();
  auto backgroundPanel = std::make_unique<Panel>(position(), size(), board::base);
  addChild("board-bg", std::move(backgroundPanel));

  const int total = gameBoard.getTileCount();
  for (int index = 0; index < total; ++index) {
    core::Tile* tile = gameBoard.getTile(index);
    if (tile == nullptr) {
      continue;
    }

    const TilePlacement placement =
        placementForIndex(position_, size_, tileNum_, index);

    std::unique_ptr<TilePanel> panel;
    if (tile->getType() == core::TileType::PROPERTY) {
      auto* propertyTile = dynamic_cast<core::PropertyTile*>(tile);
      if (propertyTile != nullptr) {
        panel = buildPropertyPanel(*propertyTile, placement, tileNum_);
      }
    } else {
      auto* actionTile = dynamic_cast<core::ActionTile*>(tile);
      if (actionTile != nullptr) {
        panel = buildActionPanel(*actionTile, placement, tileNum_);
      }
    }

    if (!panel) {
      continue;
    }

    TileInfo& info = panel->mutableSelectionInfo();
    info.tileIndex = index;
    info.isProperty = (tile->getType() == core::TileType::PROPERTY);

    panel->setOnSelected(onTileSelected_);
    addChild(std::move(panel));
  }
}

void BoardPanel::handleEvent(sf::Event& event, sf::RenderWindow& window) {
  Panel::handleEvent(event, window);
}

void BoardPanel::render(sf::RenderWindow& window) {
  Panel::render(window);

  std::unordered_map<int, std::vector<core::Player*>> playersByTile;
  for (core::Player* player : players_) {
    if (player == nullptr || player->isBankrupted()) {
      continue;
    }
    playersByTile[player->getPosition()].push_back(player);
  }

  for (auto& entry : children_) {
    auto* tile = dynamic_cast<TilePanel*>(std::get<2>(entry).get());
    if (tile == nullptr) {
      continue;
    }

    const int tileIndex = tile->selectionInfo().tileIndex;
    auto it = playersByTile.find(tileIndex);
    if (it == playersByTile.end()) {
      continue;
    }

    const sf::Vector2f tilePos = tile->position();
    const sf::Vector2f tileSize = tile->size();
    const float tokenSize = std::max(10.0f, std::min(tileSize.x, tileSize.y) * 0.24f);
    const float gap = tokenSize * 0.12f;

    for (size_t i = 0; i < it->second.size(); ++i) {
      core::Player* player = it->second[i];
      if (player == nullptr) {
        continue;
      }

      auto textureIt = avatarTextures_.find(player->getAvatar());
      if (textureIt == avatarTextures_.end() || textureIt->second == nullptr) {
        continue;
      }

      sf::Sprite token;
      token.setTexture(*textureIt->second, true);

      const sf::Vector2u source = textureIt->second->getSize();
      if (source.x == 0 || source.y == 0) {
        continue;
      }

      const float scaleX = tokenSize / static_cast<float>(source.x);
      const float scaleY = tokenSize / static_cast<float>(source.y);
      token.setScale(scaleX, scaleY);

      const float offsetX = gap + static_cast<float>(i % 2) * (tokenSize + gap);
      const float offsetY = gap + static_cast<float>(i / 2) * (tokenSize + gap);
      token.setPosition(tilePos.x + offsetX, tilePos.y + offsetY);
      window.draw(token);
    }
  }
}

void BoardPanel::update(sf::RenderWindow& window) { Panel::update(window); }

}  // namespace ui
