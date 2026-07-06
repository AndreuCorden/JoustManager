#ifndef SHOPTAB_H
#define SHOPTAB_H

#pragma once

#include "Items/Item.h"
#include <list>

class ShopTabHandler
{
public:
    ShopTabHandler();

    void startDay();

    const std::list<Item>& getBuyableItems() const { return m_buyableItems; }
    void sellItem(Item soldItem);

private:

    std::list<Item> m_buyableItems;
};

#endif