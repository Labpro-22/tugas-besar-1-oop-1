#pragma once

#include <string>

#include "core/Property.hpp"

namespace core {

class GameContext;
class Player;

enum class TileType : int { PROPERTY, ACTION };

enum class ActionTileType : int { SPECIAL, CARD, TAX, FESTIVAL };

enum class SpecialTileType : int { GO, JAIL, FREE_PARKING, GO_TO_JAIL };

enum class PropertyTileType : int { STREET, RAILROAD, UTILITY };

enum class TaxType : int { PPH, PBM };

// abstact class Tile
class Tile {
 public:
  Tile(int position, const std::string& name);
  virtual ~Tile() = default;
  virtual void onLanded(Player& p, GameContext& g) = 0;
  virtual void onPassed(Player& p, GameContext& g);
  virtual std::string getName() const;
  virtual TileType getType() const = 0;
  virtual int getPosition() const;

 protected:
  int position_;
  std::string name_;
};

class ActionTile : public Tile {
 protected:
  ActionTile(int position, const std::string& name, ActionTileType actionType);
  virtual void onLanded(Player& p, GameContext& g) override = 0;

  ActionTileType actionType_;

 public:
  TileType getType() const override;
  ActionTileType getActionTileType() const;
};

class SpecialTile : public ActionTile {
 public:
  SpecialTile(int position, const std::string& name,
              SpecialTileType specialType);
  SpecialTileType getSpecialTileType() const;

  SpecialTileType specialType_;
};

class PropertyTile : public Tile {
 public:
  PropertyTile(int position, const std::string& name,
               PropertyTileType propertyType);
  virtual Property& getProperty() = 0;
  virtual const Property& getProperty() const = 0;
  TileType getType() const override;
  PropertyTileType getPropertyTileType() const;
  void onLanded(Player& p, GameContext& g) override;

 private:
  PropertyTileType propertyType_;
};

class StreetTile : public PropertyTile {
 public:
  StreetTile(int position, const std::string& name, Street property);
  Property& getProperty() override;
  const Property& getProperty() const override;

 private:
  Street property_;
};

class UtilityTile : public PropertyTile {
 public:
  UtilityTile(int position, const std::string& name, Utility property);
  Property& getProperty() override;
  const Property& getProperty() const override;
  void onLanded(Player& p, GameContext& g) override;

 private:
  Utility property_;
};

class RailroadTile : public PropertyTile {
 public:
  RailroadTile(int position, const std::string& name, Railroad property);
  Property& getProperty() override;
  const Property& getProperty() const override;
  void onLanded(Player& p, GameContext& g) override;

 private:
  Railroad property_;
};

class GoTile : public SpecialTile {
 public:
  GoTile(int position, const std::string& name);
  void onPassed(Player& p, GameContext& g) override;
  void onLanded(Player& p, GameContext& g) override;
};

class JailTile : public SpecialTile {
 public:
  JailTile(int position, const std::string& name);
  void onLanded(Player& p, GameContext& g) override;
};

class FreeParkingTile : public SpecialTile {
 public:
  FreeParkingTile(int position, const std::string& name);
  void onLanded(Player& p, GameContext& g) override;
};

class GoToJailTile : public SpecialTile {
 public:
  GoToJailTile(int position, const std::string& name);
  void onLanded(Player& p, GameContext& g) override;
};

class TaxTile : public ActionTile {
 private:
  int flatRate_;
  int percentageRate_;
  TaxType type_;

 public:
  TaxTile(int position, const std::string& name, int flatRate,
          int percentageRate, TaxType type);
  int getFlatRate() const;
  int getPercentageRate() const;
  TaxType getTaxType() const;
  void onLanded(Player& p, GameContext& g) override;
};
class FestivalTile : public ActionTile {
 public:
  FestivalTile(int position, const std::string& name);
  void onLanded(Player& p, GameContext& g) override;
};

class CardTile : public ActionTile {
 private:
  bool isChance_;

 public:
  CardTile(int position, const std::string& name, bool isChance);
  bool isChance() const;
  void onLanded(Player& p, GameContext& g) override;
};

}  // namespace core
