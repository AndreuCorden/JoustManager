#include "Knight.h"

Knight Knight::generateRandomKnight()
{
    return Knight("Sir Placeholder", "Camelot", "The Tester", 180, 85);
}

int Knight::attack()
{
    return rightHandWeapon.getBonus();
}