#pragma once

#include <string> 
#include "./Player.hpp"
#include "./Property.hpp"
#include "../logic/Game.hpp" 
using namespace std; 

namespace core {

// abstact class Tile  
class Tile {
	public: 
		virtual void onLanded(); 
		virtual string getType(); 
		virtual void onPassed(); 
		virtual string getName(); 
		virtual int getPosition(); 

	protected: 
		int position_;
		string name_; 
};

class ActionTile : public Tile { 
	protected: 
		virtual void onLanded(Player& p, logic::Game& g); 
}; 

class PropertyTile : public Tile { 
	public: 
		virtual Property getProperty(); 
		virtual void onLanded(Player& p, logic::Game& g); 

	protected:  						
		Property property_; 	
}; 

class UtilityTile : public PropertyTile { 
	public: 
		void onLanded(Player& p, logic::Game& g) override ; 
		virtual string getType() override; 
}; 

class RailroadTile : public PropertyTile { 
	public: 
		void onLanded(Player&, logic::Game&) override;
		virtual string getType() override; 

};

class GoTile : public ActionTile { 
	public: 
		void onLanded(Player&, logic::Game&) override; 
		virtual string getType() override; 
};

class JailTile : public ActionTile { 
	public: 
		void onLanded(Player&, logic::Game&) override; 
		virtual string getType() override; 
};


class FreeParkingTile : public ActionTile { 
	public: 
		void onLanded(Player&, logic::Game&) override; 
		virtual string getType() override; 
};


class GoToJailTile: public ActionTile { 
	public: 
		void onLanded(Player&, logic::Game&) override; 
		virtual string getType() override; 
};


class TaxTile: public ActionTile { 
	private: 
		// TaxType type_; 
		int rate_; 

	public: 
		void onLanded(Player&, logic::Game&) override; 
		virtual string getType() override; 
}; 
class FestivalTile: public ActionTile { 
	public: 
		void onLanded(Player&, logic::Game&) override; 
		virtual string getType() override; 
};

class CardTile: public ActionTile { 
	private: 
		// CardType type_; 
		// CardDeck<ActionCard>* deck; 
	public: 
		void onLanded(Player&, logic::Game&) override; 
		virtual string getType() override; 
};

}  // namespace core
