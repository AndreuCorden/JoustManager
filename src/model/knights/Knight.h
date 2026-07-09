#ifndef KNIGHT_H
#define KNIGHT_H

#include <string>
#include <vector>
#include "model/items/Item.h"

class Knight
{
public:
    // Regular constructor used by our generator
    Knight(std::string n, std::string o, std::string t, int h, int w, int c)
        : name(n), origin(o), title(t), height(h), weight(w), level(1), cost(c){}

    // The Magic Generator: 'static' means we can call this without an existing Knight object
    static Knight generateRandomKnight();

    // Getters so your UI can display the stats
    std::string getName() const { return name; }
    std::string getOrigin() const { return origin; }
    std::string getTitle() const { return title; }
    int getHeight() const { return height; }
    int getWeight() const { return weight; }

    Item getArmour() const
    {
        return armour;
    }
    
    void equipArmour(const Item & newArmour)
    {
        armour = newArmour;
    }

    Item getRightHandWeapon() const
    {
        return rightHandWeapon;
    }

    void equipRightHandWeapon(const Item & newWeapon)
    {
        rightHandWeapon = newWeapon;
    }

    int attack();

    void setCost(int newCost)
    {
        cost = newCost;
    }

    int getCost() const
    {
        return cost;
    }

private:
    std::string name;
    std::string origin;
    std::string title;
    int height;
    int weight;
    int level;
    int cost;
    Item armour;
    Item rightHandWeapon;
};

#endif