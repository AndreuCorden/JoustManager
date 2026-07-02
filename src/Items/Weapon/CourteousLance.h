#ifndef _Courteous_Lance
#define _Courteous_Lance

#include "Items/Item.h"

class CourteousLance : public Item
{
public:
    CourteousLance()
    : Item("Courteous Lance", 10, 25, Right_Hand, Weapon)
    {}
private:
};

#endif