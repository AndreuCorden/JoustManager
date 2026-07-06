#include <QRandomGenerator>

#include "Items/ShopTabHandler.h"
#include "Items/Armour/GothicArmour.h"
#include "Items/Weapon/CourteousLance.h"

ShopTabHandler::ShopTabHandler()
{
    startDay();
}

void ShopTabHandler::startDay()
{
    m_buyableItems.clear();

    auto *rand = QRandomGenerator::global();

    int numItems = rand->bounded(1,5);
    for (int i = 0; i < numItems; ++i)
    {
        Item item;
        if(rand->bounded(2)%2 == 0)
        {
            item = GothicArmour();
        }
        else
        {
            item = CourteousLance();
        }
        m_buyableItems.push_back(item);
    }
}