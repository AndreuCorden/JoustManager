#include "Knight.h"

Knight Knight::generateRandomKnight()
{
    return Knight("Sir Placeholder", "Camelot", "The Tester", 180, 85, 100);
}

int Knight::attack()
{
    return rightHandWeapon.getBonus();
}