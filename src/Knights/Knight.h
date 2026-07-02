#ifndef _Knight
#define _Knight

#include <string>
#include <vector>
#include "Items/Item.h"

class Knight
{
public:
    // Regular constructor used by our generator
    Knight(std::string n, std::string o, std::string t, int h, int w)
        : name(n), origin(o), title(t), height(h), weight(w), level(1) {}

    // The Magic Generator: 'static' means we can call this without an existing Knight object
    static Knight generateRandomKnight();

    // Getters so your UI can display the stats
    std::string getName() const { return name; }
    std::string getOrigin() const { return origin; }
    std::string getTitle() const { return title; }
    int getHeight() const { return height; }
    int getWeight() const { return weight; }

    void setArmour(const Item & newArmour)
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

private:
    std::string name;
    std::string origin;
    std::string title;
    int height;
    int weight;
    int level;
    Item armour;
    Item rightHandWeapon;
};

#endif