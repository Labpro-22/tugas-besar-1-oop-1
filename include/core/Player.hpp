#pragma once 

#include <string> 
#include <vector>
using namespace std;

namespace logic {class Game;}
namespace core {
class Property; 

class Player { 
	protected: 
		string name_; 
		string token_; 
		int balance_; 
		int position_; 
		bool inJail_; 
		bool isBankrupt_; 
		int jailTurns_; 
		vector<Property*> ownedProperties_;
		//vector<ActionCard*> heldCards_; 
	public: 
		Player(string name); // assume that others will be inputted as defaults and there is no default names. 
		virtual void takeTurn(logic::Game& g); 
		virtual bool isHuman(); 
		int getNetWorth() const;
		bool canAfford(int amount) const;
		void goToJail(); 
		void releaseFromJail(); 
		void addProperty(Property* p); 
		void removeProperty(Property* p); 
		Player& operator+(int amount); // terima uang
		Player& operator-=(int amount); // keluar uang
		bool operator<(const Player& other) const; 
		bool operator>(const Player& other) const; 
		bool operator==(const Player& other) const; 
		string getName() const;
		int getBalance() const;
		int getPosition() const;
		bool getInJail() const;
		bool getIsBankrupt() const;
		vector<Property*>& getOwnedProperties();
		virtual ~Player() = default;
};

class HumanPlayer : public Player {
	public: 
		HumanPlayer(string name) : Player(move(name)){};
		void takeTurn(logic::Game& g) override;
		bool isHuman() override; 
};

} 



