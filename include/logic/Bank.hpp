 #pragma once

namespace core { class Player; }

namespace logic {

class Bank {
private:
    int totalMoney_; 
    int housesAvailable_;
    int hotelsAvailable_;

public:
    Bank(int initialMoney = 999999, int initialHouses = 32, int initialHotels = 12);
    ~Bank() = default;

    bool canBuyHouse(int count) const;
    bool canBuyHotel(int count) const;
    
    void sellHouse(int count);
    void sellHotel(int count);
    
    void receiveHouse(int count);
    void receiveHotel(int count);
    
    void pay(core::Player& p, int amount);
    void receive(int amount);
};

} // namespace logic
