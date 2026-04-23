#include "core/Tiles.hpp"

namespace core {
Tile::Tile(int position, const std::string& name) : position_(position), name_(name) {}; 


void Tile::onPassed(Player& p, GameContext& g) {
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

void PropertyTile::onLanded(Player& p, GameContext& g){ 

	if (!property_) return; 

	if (property_->isAvailable()){ 
		// purchase 
		g.offerProperty(&p, property_); 
	}else if (property_->getOwner() != &p && !property_->isMortgagedStatus()){
		// rent 
		g.chargeRent(&p, property_);
	} 
} 

UtilityTile::UtilityTile(int position, const std::string& name, Property* property) : PropertyTile(position, name, property) {}

void UtilityTile::onLanded(Player& p, GameContext& g){ 
	if (property_->isAvailable()){ 
		property_->setOwner(&p); 
		p.addProperty(property_);
	} else if (property_->getOwner() != &p && !property_->isMortgagedStatus()){
		// rent 
		g.chargeRent(&p, property_);
	} 
} 

std::string UtilityTile::getType() const { return "Utility Tile";} 
RailroadTile::RailroadTile(int position, const std::string& name, Property* property) : PropertyTile(position, name, property) {}

void RailroadTile::onLanded(Player& p, GameContext& g) {
    if (property_->isAvailable()) {
        property_->setOwner(&p);
        p.addProperty(property_);
		g.logEvent("RAILROAD", p, *property_, 0);
    } else if (property_->getOwner() != &p && !property_->isMortgagedStatus()) {
   	// rent  
		g.chargeRent(&p, property_);
    }
}

std::string RailroadTile::getType() const { return "RailroadTile"; }


GoTile::GoTile(int position, const std::string& name) : ActionTile(position, name) {}

void GoTile::onLanded(Player& p, GameContext& g) {
	// get salary added to their inventoryt	
	g.payPlayerFromBank(p, g.getGoSalary());
	g.logEvent("GO_SALARY", p, g.getGoSalary());
}

void GoTile::onPassed(Player& p, GameContext &g){ 
	// get salary again 
	g.payPlayerFromBank(p, g.getGoSalary());
	g.logEvent("GO_SALARY", p, g.getGoSalary());
}


std::string GoTile::getType() const { return "Go Tile"; }

FreeParkingTile::FreeParkingTile(int position, const std::string& name) : ActionTile(position, name) {}

void FreeParkingTile::onLanded(Player& p, GameContext& g) {
	// nothing happens here 
}

std::string FreeParkingTile::getType() const { return "FreeParkingTile"; }

JailTile::JailTile(int position, const std::string& name) : ActionTile(position, name) {}
void JailTile::onLanded([[maybe_unused]] Player& p, [[maybe_unused]] GameContext& g) {
    // just visiting
}
std::string JailTile::getType() const { return "Jail Tile"; } 

GoToJailTile::GoToJailTile(int position, const std::string& name) : ActionTile(position, name) {}

void GoToJailTile::onLanded(Player& p, GameContext& g) { 
	// go to jail 
	// p.sendToJail(p); 
	g.logEvent("GO_TO_JAIL", p, 0);
} 

std::string GoToJailTile::getType() const { return "GoToJail Tile";} 

TaxTile::TaxTile(int position, const std::string& name, int rate, bool isPercentage) : ActionTile(position, name), rate_(rate), isPercentage_(isPercentage) {}

void TaxTile::onLanded(Player& p, GameContext& g) {
	int taxAmount = isPercentage_ ? (p.getBalance() * rate_ / 100) : rate_;
	g.chargeTax(&p, taxAmount, false);
}

std::string TaxTile::getType() const {return "Tax Tile";} 

FestivalTile::FestivalTile(int position, const std::string& name) : ActionTile(position, name) {} ; 

void FestivalTile::onLanded([[maybe_unused]] Player& p,[[maybe_unused]] GameContext& g) {
	// pick property 
	g.activateFestival(&p);
}


std::string FestivalTile::getType() const { return "FestivalTile"; }


CardTile::CardTile(int position, const std::string& name, bool isChance) : ActionTile(position, name), isChance_(isChance) {}

void CardTile::onLanded([[maybe_unused]]Player& p,[[maybe_unused]] GameContext& g) {
    if (isChance_) {
    	// draw chance 
    } else {
    }
}

std::string CardTile::getType() const { return "CardTile"; }
} // namespace core
