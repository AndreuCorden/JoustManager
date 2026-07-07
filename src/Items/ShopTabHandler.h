#ifndef SHOPTAB_H
#define SHOPTAB_H

#pragma once

#include "Items/Item.h"

#include <list>
#include <random>

class ShopTabHandler
{
public:
    ShopTabHandler();

    void startDay();

    const std::list<Item>& getBuyableItems() const { return m_buyableItems; }
    void sellItem(const Item& soldItem);

private:
    std::list<Item> m_buyableItems;

    std::default_random_engine m_random_engine;
};

#endif