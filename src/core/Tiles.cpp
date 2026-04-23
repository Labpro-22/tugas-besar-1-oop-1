#include "core/Tiles.hpp"

namespace core {
Tile::Tile(int position, const std::string& name) : position_(position), name_(name) {}; 


void Tile::onPassed(Player& p, GameContext& g) {
	// do nothing, default
} 

std::string Tile::getName() const { 
	return this->name_; 
} 

TileType Tile::getType() const { 
	return TileType::BASE;
} 

int Tile::getPosition() const { 
	return position_; 
} 

ActionTile::ActionTile (int position, const std::string& name) : Tile(position, name) {} 


PropertyTile::PropertyTile(int position, const std::string& name, std::unique_ptr<Property> property) : Tile(position, name), property_(std::move(property)) {} 

Property* PropertyTile::getProperty() const {return property_.get();} 

void PropertyTile::onLanded(Player& p, GameContext& g){ 

	if (!property_) return; 

	if (property_->isAvailable()){ 
		// purchase 
		g.offerProperty(&p, property_.get()); 
	}else if (property_->getOwner() != &p && !property_->isMortgagedStatus()){
		// rent 
		g.chargeRent(&p, property_.get());
	} 
} 

UtilityTile::UtilityTile(int position, const std::string& name, std::unique_ptr<Property> property) : PropertyTile(position, name, std::move(property)) {}

void UtilityTile::onLanded(Player& p, GameContext& g){ 
	if (property_->isAvailable()){ 
		property_->setOwner(&p); 
		p.addProperty(property_.get());
	} else if (property_->getOwner() != &p && !property_->isMortgagedStatus()){
		// rent 
		g.chargeRent(&p, property_.get());
	} 
} 

TileType UtilityTile::getType() const { return TileType::UTILITY;} 

RailroadTile::RailroadTile(int position, const std::string& name, std::unique_ptr<Property> property) : PropertyTile(position, name, std::move(property)) {}

void RailroadTile::onLanded(Player& p, GameContext& g) {
    if (property_->isAvailable()) {
        property_->setOwner(&p);
        p.addProperty(property_.get());
		g.logEvent("RAILROAD", p, *property_, 0);
    } else if (property_->getOwner() != &p && !property_->isMortgagedStatus()) {
   	// rent  
		g.chargeRent(&p, property_.get());
    }
}

TileType RailroadTile::getType() const { return TileType::RAILROAD; }


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


TileType GoTile::getType() const { return TileType::GO; }

FreeParkingTile::FreeParkingTile(int position, const std::string& name) : ActionTile(position, name) {}

void FreeParkingTile::onLanded(Player& p, GameContext& g) {
	// nothing happens here 
}

TileType FreeParkingTile::getType() const { return TileType::FREE_PARKING; }

JailTile::JailTile(int position, const std::string& name) : ActionTile(position, name) {}
void JailTile::onLanded([[maybe_unused]] Player& p, [[maybe_unused]] GameContext& g) {
    // just visiting
}
TileType JailTile::getType() const { return TileType::JAIL; } 

GoToJailTile::GoToJailTile(int position, const std::string& name) : ActionTile(position, name) {}

void GoToJailTile::onLanded(Player& p, GameContext& g) { 
	// go to jail 
	// p.sendToJail(p); 
	p.goToJail();
	g.logEvent("GO_TO_JAIL", p, 0);
} 

TileType GoToJailTile::getType() const { return TileType::GO_TO_JAIL;} 

TaxTile::TaxTile(int position, const std::string& name, int rate, bool isPercentage) : ActionTile(position, name), rate_(rate), isPercentage_(isPercentage) {}

void TaxTile::onLanded(Player& p, GameContext& g) {
	int taxAmount = isPercentage_ ? (p.getBalance() * rate_ / 100) : rate_;
	g.chargeTax(&p, taxAmount, false);
}

TileType TaxTile::getType() const {return TileType::TAX;} 

bool TaxTile::isPercentage() const { return isPercentage_; }
int TaxTile::getRate() const { return rate_; }

FestivalTile::FestivalTile(int position, const std::string& name) : ActionTile(position, name) {} ; 

void FestivalTile::onLanded([[maybe_unused]] Player& p,[[maybe_unused]] GameContext& g) {
	// pick property 
	g.activateFestival(&p);
}


TileType FestivalTile::getType() const { return TileType::FESTIVAL; }


CardTile::CardTile(int position, const std::string& name, bool isChance) : ActionTile(position, name), isChance_(isChance) {}

void CardTile::onLanded([[maybe_unused]]Player& p,[[maybe_unused]] GameContext& g) {
    if (isChance_) {
    	// draw chance 
    } else {
    }
}

bool CardTile::isChance() const { return isChance_; }

TileType CardTile::getType() const { return TileType::CARD; }


} // namespace core
