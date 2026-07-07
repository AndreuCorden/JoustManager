#ifndef GOTHICARMOUR_H
#define GOTHICARMOUR_H

#include "model/items/Item.h"

class GothicArmour : public Item
{
public:
    GothicArmour()
    : Item("Gothic Armour", 25, 100, Body, Armour)
    {}
private:
};

#endif