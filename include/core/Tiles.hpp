#pragma once

#include <string> 
#include "core/Player.hpp"
#include "core/Property.hpp"
#include "core/GameContext.hpp" 
#include <memory> 


namespace logic { class Game; } 

namespace core {

enum class TileType : int {
    BASE,
    ACTION,
    PROPERTY,
    UTILITY,
    RAILROAD,
    GO,
    JAIL,
    FREE_PARKING,
    GO_TO_JAIL,
    TAX,
    FESTIVAL,
    CARD
};

// abstact class Tile  
class Tile {
	public: 
		Tile(int position, const std::string& name); 
		virtual ~Tile() = default; 
		virtual void onLanded(Player&p, GameContext& g) = 0; 
		virtual void onPassed(Player& p, GameContext& g); 
		virtual std::string getName() const; 
		virtual TileType getType() const ; 
		virtual int getPosition() const; 

		  virtual Property *getProperty() const { return nullptr; }
	protected: 
		int position_;
		std::string name_; 
};

class ActionTile : public Tile { 
	protected:
		ActionTile(int position, const std::string& name ); 
		virtual void onLanded(Player& p, GameContext& g) override = 0; 
}; 

class PropertyTile : public Tile { 
	public: 
		PropertyTile(int position, const std::string& name, std::unique_ptr<Property> property); 
		virtual Property* getProperty() const; 
		virtual void onLanded(Player& p, GameContext& g); 

	protected:  						
		std::unique_ptr<Property> property_; 	
}; 

class UtilityTile : public PropertyTile { 
	public: 
		UtilityTile(int position, const std::string& name, std::unique_ptr<Property> property); 
		void onLanded(Player& p, GameContext& g) override ; 
		virtual TileType getType() const override; 
}; 

class RailroadTile : public PropertyTile { 
	public: 
		RailroadTile(int position, const std::string& name, std::unique_ptr<Property> property); 
void onLanded(Player& p, GameContext& g) override;
		virtual TileType getType() const override; 

};

class GoTile : public ActionTile { 
	public: 
		GoTile(int position, const std::string& name);
		void onPassed(Player& p, GameContext& g) override; 
		void onLanded(Player& p, GameContext& g) override; 
		virtual TileType getType() const override;
};

class JailTile : public ActionTile { 
	public: 
		JailTile(int position, const std::string& name);
		void onLanded(Player& p, GameContext& g) override; 
		virtual TileType getType() const  override;
};


class FreeParkingTile : public ActionTile { 
	public: 
		FreeParkingTile(int position, const std::string& name);
		void onLanded(Player& p, GameContext& g) override; 
		virtual TileType getType() const override;
};


class GoToJailTile: public ActionTile { 
	public: 
		GoToJailTile(int position, const std::string& name);
		void onLanded(Player& p, GameContext& g) override; 
		virtual TileType getType() const override;
};


class TaxTile: public ActionTile { 
	private: 
		int flatRate_; 
		int percentageRate_;
		TaxType type_;

	public: 
		TaxTile(int position, const std::string& name, int flatRate, int percentageRate, TaxType type);
		int getFlatRate() const;
		int getPercentageRate() const;
		TaxType getTaxType() const;
		void onLanded(Player& p, GameContext& g) override; 
		virtual TileType getType() const override; 

	};
class FestivalTile: public ActionTile { 
	public: 

		FestivalTile(int position, const std::string& name);
		void onLanded(Player& p, GameContext& g) override; 
		virtual TileType getType() const override;
};

class CardTile: public ActionTile { 
	private: 
		bool isChance_; 
	public: 
		CardTile(int position, const std::string& name, bool isChance);
		bool isChance() const;
		void onLanded(Player& p, GameContext& g) override; 
		virtual TileType getType() const override;
};

}  // namespace core
