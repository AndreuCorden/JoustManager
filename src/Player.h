#ifndef PLAYER_H
#define PLAYER_H

#pragma once
#include <string>
#include <list>
#include <vector>
#include "model/knights/Knight.h"
#include "model/items/Item.h"

class Player
{
public:
    Player();

    // Delete copy constructor and assignment operator so nobody accidentally copies the player
    Player(const Player &) = delete;
    void operator=(const Player &) = delete;

    // Roster Management
    std::list<Knight> &getRoster() { return knightsInRoster; }
    void addKnight(const Knight &knight) { knightsInRoster.push_back(knight); }

    // Inventory Management (Changed from Item& to Item for standard vectors)
    std::vector<Item> &getWeapons() { return weaponInventory; }
    std::vector<Item> &getArmour() { return armorInventory; }

    void addWeapon(const Item &w) { weaponInventory.push_back(w); }
    void addArmour(const Item &a) { armorInventory.push_back(a); }

private:
    std::list<Knight> knightsInRoster;
    std::vector<Item> weaponInventory;
    std::vector<Item> armorInventory;
};

#endif