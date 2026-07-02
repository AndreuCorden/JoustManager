#ifndef _Gothic_Armour
#define _Gothic_Armour

#include "Items/Item.h"

class GothicArmour : public Item
{
public:
    GothicArmour()
    : Item("Gothic Armour", 25, 100, Body, Armour)
    {}
private:
};

#endif