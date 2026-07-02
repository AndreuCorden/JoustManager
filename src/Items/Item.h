#ifndef _Item
#define _Item

#include <string>

enum BodyPart{Body, Right_Hand, Left_Hand, TwoHanded, Either_Hand};
enum ItemType{Armour, Weapon};

class Item
{
public:
    Item()
    : name("Blank Item") , bonus(0) , cost(0) , heldIn(Body) , typeOfItem (Armour)
    {}
    
    Item(std::string newName, int newBonus, int newCost, BodyPart newHeldIn, ItemType newTypeOfItem)
    : name(newName) , bonus(newBonus) , cost(newCost) , heldIn(newHeldIn) , typeOfItem(newTypeOfItem)
    {}

    int getBonus()
    {
        return bonus;
    }
protected:
    std::string name;
    int bonus;
    int cost;
    BodyPart heldIn;
    ItemType typeOfItem;
};

#endif