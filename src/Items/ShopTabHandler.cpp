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

void ShopTabHandler::sellItem(const Item& soldItem)
{
    // Loop using an iterator to find and wipe out the target element item block
    for (auto it = m_buyableItems.begin(); it != m_buyableItems.end(); ++it)
    {
        if (it->getName() == soldItem.getName() && it->getBonus() == soldItem.getBonus())
        {
            m_buyableItems.erase(it); // Erase the matching entry item layout
            break; // Stop looking after removing one instance matching this signature
        }
    }
}