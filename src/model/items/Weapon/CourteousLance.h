#ifndef COURTEOUSLANCE_H
#define COURTEOUSLANCE_H

#include "model/items/Item.h"

class CourteousLance : public Item
{
public:
    CourteousLance()
    : Item("Courteous Lance", 10, 25, Right_Hand, Weapon)
    {}
private:
};

#endif