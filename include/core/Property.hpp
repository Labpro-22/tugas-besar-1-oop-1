#pragma once 

#include <string> 
#include "Player.hpp" 
#include <vector> 
#include <map>

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
	GREY,
	PURPLE,
	LIGHT_GREEN,
	LAVENDER,
	WHITE,
	BLACK,
    DEFAULT
};

enum class PropertyType { 
	STREET, 
	RAILROAD, 
	UTILITY
};

class Property { 
	protected: 
		std::string name_; 
		Player* owner_; 
		bool isMortgaged_; 
		int price_; 
		int mortgageValue_; 
		int festMultiplier_;
		int festDuration_;

	public: 
		Property();
		Property(const std::string& name, int price, int mortgageValue);

		virtual int calculateRent(int diceRoll, int ownedSameType, bool hasMonopoly) const = 0; 
		virtual PropertyType getType() const = 0;
		void mortgage(); 
		void unmortgage(); 
		void setOwner(Player* p); 
		Player* getOwner() const; 
		bool isOwned() const; 
		bool isAvailable() const; 
		bool isMortgagedStatus() const;
		const std::string& getName() const;
		int getPrice() const;
		int getMortgageValue() const;
		void applyFestival();
		void tickFestival();
		int getFestMultiplier() const;
		int getFestDuration() const;
		void setFestivalState(int mult, int dur);
		
		virtual ~Property() = default;
}; 
class Street : public Property { 
	private: 
		ColorGroup colorGroup_; 
		int houseCount_; 
		int hotelCount_; 
		std::vector<int> rentValues_; 
		int houseCost_; 
		int hotelCost_; 
	public: 
		Street(const std::string& name, int price, int mortgageValue, ColorGroup colorGroup, std::vector<int> rentValues, int houseCost, int hotelCost);
		int calculateRent(int diceRoll, int ownedSameType, bool hasMonopoly) const override; 
		PropertyType getType() const override ; 
		
		void build();
		void demolish(int n);

		void setLevel(int level);

		bool canBuild(bool hasMonopoly) const; 
		
		ColorGroup getColorGroup() const;
    	int getHouseCount() const;
    	int getHotelCount() const;
    	int getHouseCost() const;
		int getHotelCost() const;

};

class Railroad : public Property {
	private: 
		int baseFare_; 
		static std::map<int, int> rentTable_;
	public: 
		Railroad(const std::string& name, int mortgageValue, int baseFare);
		static void setRentTable(const std::map<int, int>& table);
		int calculateRent(int diceRoll, int ownedSameType, bool hasMonopoly) const override; 
		PropertyType getType() const override ; 
		int getBaseFare() const;
		
};

class Utility : public Property { 
	private: 
		static std::map<int, int> multiplierTable_; // jmlh utility -> faktor pengali
	public: 
		Utility(const std::string& name, int mortgageValue);
		static void setMultiplierTabel(const std::map<int, int>& table);
		int calculateRent(int diceRoll, int ownedSameType, bool hasMonopoly) const override; 
		PropertyType getType() const override; 
		int getCurrentMultiplier(int ownedUtilities) const;
	
}; 

}