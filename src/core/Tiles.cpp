#include "core/Tiles.hpp"

namespace core {
Tile::Tile(int position, const std::string& name) : position_(position), name_(name) {}; 


void Tile::onPassed(Player& p, logic::Game& g) {
	// do nothing, default
} 

std::string Tile::getName() const { 
	return this->name_; 
} 

std::string Tile::getType() const { 
	return "Tile";
} 

int Tile::getPosition() const { 
	return position_; 
} 

ActionTile::ActionTile (int position, const std::string& name) : Tile(position, name) {} 


PropertyTile::PropertyTile(int position, const std::string& name, Property* property) : Tile(position, name), property_(property) {} 

Property* PropertyTile::getProperty() const {return property_;} 

void PropertyTile::onLanded(Player& p, logic::Game& g){ 
	if (property_->isAvailable()){ 
		// purchase 
	}else if (property_->getOwner() != &p && !property_->isMortgagedStatus()){
		// rent 
	} 
} 

UtilityTile::UtilityTile(int position, const std::string& name, Property* property) : PropertyTile(position, name, property) {}

void UtilityTile::onLanded(Player& p, logic::Game& g){ 
	if (property_->isAvailable()){ 
		property_->setOwner(&p); 
		p.addProperty(property_);
	} else if (property_->getOwner() != &p && !property_->isMortgagedStatus()){
		// rent 
	} 
} 

std::string UtilityTile::getType() const { return "Utility Tile";} 
RailroadTile::RailroadTile(int position, const std::string& name, Property* property) : PropertyTile(position, name, property) {}

void RailroadTile::onLanded(Player& p, logic::Game& g) {
    if (property_->isAvailable()) {
        property_->setOwner(&p);
        p.addProperty(property_);
    } else if (property_->getOwner() != &p && !property_->isMortgagedStatus()) {
   	// rent  
    }
}

std::string RailroadTile::getType() const { return "RailroadTile"; }


GoTile::GoTile(int position, const std::string& name) : ActionTile(position, name) {}

void GoTile::onLanded(Player& p, logic::Game& g) {
	// get salary added to their inventoryt	
}

void GoTile::onPassed(Player& p, logic::Game &g){ 
	// get salary again 
}


std::string GoTile::getType() const { return "Go Tile"; }

FreeParkingTile::FreeParkingTile(int position, const std::string& name) : ActionTile(position, name) {}

void FreeParkingTile::onLanded(Player& p, logic::Game& g) {
	// nothing happens here 
}

std::string FreeParkingTile::getType() const { return "FreeParkingTile"; }

JailTile::JailTile(int position, const std::string& name) : ActionTile(position, name) {}
void JailTile::onLanded([[maybe_unused]] Player& p, [[maybe_unused]] logic::Game& g) {
    // just visiting
}
std::string JailTile::getType() const { return "Jail Tile"; } 

GoToJailTile::GoToJailTile(int position, const std::string& name) : ActionTile(position, name) {}

void GoToJailTile::onLanded(Player& p, logic::Game& g) { 
	// go to jail 
} 

std::string GoToJailTile::getType() const { return "GoToJail Tile";} 

TaxTile::TaxTile(int position, const std::string& name, int rate, bool isPercentage) : ActionTile(position, name), rate_(rate), isPercentage_(isPercentage) {}

void TaxTile::onLanded(Player& p, logic::Game& g) {
	if (isPercentage_) {
		// pph 
	} else {
		// pbm 
	}
}

std::string TaxTile::getType() const {return "Tax Tile";} 

FestivalTile::FestivalTile(int position, const std::string& name) : ActionTile(position, name) {} ; 

void FestivalTile::onLanded([[maybe_unused]] Player& p,[[maybe_unused]] logic::Game& g) {
	// pick property 
}


std::string FestivalTile::getType() const { return "FestivalTile"; }


CardTile::CardTile(int position, const std::string& name, bool isChance) : ActionTile(position, name), isChance_(isChance) {}

void CardTile::onLanded([[maybe_unused]]Player& p,[[maybe_unused]] logic::Game& g) {
    if (isChance_) {
    	// draw chance 
    } else {
    }
}

std::string CardTile::getType() const { return "CardTile"; }
} // namespace core
