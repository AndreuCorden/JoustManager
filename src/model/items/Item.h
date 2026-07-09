#ifndef ITEM_H
#define ITEM_H

#include <string>

class Item
{
public:
    enum BodyPart
    {
        Body,
        Right_Hand,
        Left_Hand,
        TwoHanded,
        Either_Hand
    };
    enum ItemType
    {
        Armour,
        Weapon
    };

    Item()
        : name("None"), bonus(0), cost(0), heldIn(Body), typeOfItem(Armour)
    {
    }

    Item(std::string newName, int newBonus, int newCost, BodyPart newHeldIn, ItemType newTypeOfItem)
        : name(newName), bonus(newBonus), cost(newCost), heldIn(newHeldIn), typeOfItem(newTypeOfItem)
    {
    }

    std::string getName() const
    {
        return name;
    }

    int getBonus() const
    {
        return bonus;
    }

    const ItemType& getItemType() const
    {
        return typeOfItem;
    }

    const int getCost() const
    {
        return cost;
    }

protected:
    std::string name;
    int bonus;
    int cost;
    BodyPart heldIn;
    ItemType typeOfItem;
};

#endif