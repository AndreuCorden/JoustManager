#ifndef _Player
#define _Player

#pragma once
#include <string>
#include <list>
#include <vector>
#include "Knights/Knight.h"
#include "model/items/Item.h"

class Player {
public:
    // This static method provides global access to the single player instance
    static Player& getInstance() {
        static Player instance; // Guaranteed to be destroyed, instantiated on first use
        return instance;
    }

    // Delete copy constructor and assignment operator so nobody accidentally copies the player
    Player(const Player&) = delete;
    void operator=(const Player&) = delete;

    // Gold Management
    int getGold() const { return gold; }
    void modifyGold(int amount) { gold += amount; }

    // Roster Management
    std::list<Knight>& getRoster() { return knightsInRoster; }
    void addKnight(const Knight& knight) { knightsInRoster.push_back(knight); }

    // Inventory Management (Changed from Item& to Item for standard vectors)
    std::vector<Item>& getWeapons() { return weaponInventory; }
    std::vector<Item>& getArmour() { return armorInventory; }
    
    void addWeapon(const Item& w) { weaponInventory.push_back(w); }
    void addArmour(const Item& a) { armorInventory.push_back(a); }

private:
    Player(); // Constructor is now PRIVATE

    int gold;
    std::list<Knight> knightsInRoster;
    std::vector<Item> weaponInventory;
    std::vector<Item> armorInventory;
};

#endif