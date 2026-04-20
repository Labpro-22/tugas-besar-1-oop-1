#pragma once 

#include <string> 
#include "Player.hpp" 
#include "Game.hpp" 
#include <vector> 
#include <map>

using namespace std; 
namespace logic {class Game;}
namespace core {class Player;}
namespace core { 
enum class ColorGroup {
    BROWN,
    LIGHT_BLUE,
    PINK,
    ORANGE,
    RED,
    YELLOW,
    GREEN,
    DARK_BLUE,
    DEFAULT  // utk Railroad, Utility, petak aksi
};

enum class PropertyType { 
	STREET, 
	RAILROAD, 
	UTILITY
};

class Property { 
	private: 
		string name_; 
		Player* owner_; 
		bool isMortgaged_; 
		int price_; 
		int mortgageValue_; 
	public: 
		Property();
		Property(string name, int price, int mortgageValue);

		virtual int calculateRent(logic::Game& g) = 0; 
		virtual string getType() const = 0; 
		void mortgage(); 
		void unmortgage(); 
		void setOwner(Player* p); 
		Player* getOwner() const; 
		bool isOwned() const; 
		bool isAvailable() const; 
		bool isMortgagedStatus() const;
		string getName() const;
		int getPrice() const;
		int getMortgageValue() const;
		virtual ~Property() = default;
}; 
class Street : public Property { 
	private: 
		ColorGroup colorGroup_; 
		int houseCount_; 
		int hotelCount_; 
		vector<int> rentValues_; 
		int houseCost_; 
		int hotelCost_; 
		int festivalMultiplier_;
		int festivalDuration_;
	public: 
		Street(string name, int price, int mortgageValue, ColorGroup colorGroup, vector<int> rentValues, int houseCost, int hotelCost);
		int calculateRent(logic::Game& g) override; 
		string getType() const override ; 
		void buildHouse(); 
		void buildHotel(); 
		void demolish(int n);
		bool canBuild(logic::Game& g) const; 
		bool hasMonopoly(logic::Game& g) const; 
		void applyFestival();
		void tickFestival();
		ColorGroup getColorGroup() const;
    	int getHouseCount() const;
    	int getHotelCount() const;
    	int getFestivalMultiplier() const;
    	int getFestivalDuration() const;
    	void setFestivalState(int mult, int dur);

};

class Railroad : public Property {
	private: 
		int baseFare_; 
	public: 
		Railroad(string name, int mortgageValue, int baseFare);
		int calculateRent(logic::Game& g) override; 
		string getType() const override ; 
		int getBaseFare() const;
		
};

class Utility : public Property { 
	private: 
		map<int, int> multiplierTable_; // jmlh utility -> faktor pengali
	public: 
		Utility(string name, int mortgageValue, map<int, int> multiplierTable);
		int calculateRent(logic::Game& g) override; 
		string getType() const override ; 
		int getCurrentMultiplier(logic::Game& g) const;
	

}; 
} 


