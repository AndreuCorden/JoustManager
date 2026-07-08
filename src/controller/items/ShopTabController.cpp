#include "controller/items/ShopTabController.h"
#include "Player.h"

ShopTabController::ShopTabController(QWidget *parent)
    : m_shopTabHandler(new ShopTabHandler())
{
    m_shopView = new ShopView(parent);
    m_shopView->populateList(m_shopTabHandler->getBuyableItems());

    connect(m_shopView, &ShopView::itemPurchased, this, &ShopTabController::handleItemPurchase);
}

void ShopTabController::handleItemPurchase(const Item &item)
{
    int cost = item.getCost();
    bool approved = false;

    emit requestPayment(cost, approved);

    if (!approved)
    {
        // Optional: Trigger a UI toast message or error dialog here
        return;
    }

    if (item.getItemType() == Item::ItemType::Armour)
    {
        Player::getInstance().addArmour(item);
    }
    else
    {
        Player::getInstance().addWeapon(item);
    }

    // 1. Remove the item from your backend handler state
    m_shopTabHandler->sellItem(item);

    // 2. Force the view to repaint and sync with the clean backend data list
    m_shopView->populateList(m_shopTabHandler->getBuyableItems());
}

void ShopTabController::startDay()
{
    m_shopTabHandler->startDay();
    m_shopView->populateList(m_shopTabHandler->getBuyableItems());
}