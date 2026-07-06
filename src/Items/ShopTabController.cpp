#include "Items/ShopTabController.h"

ShopTabController::ShopTabController(QWidget *parent)
: m_shopTabHandler(new ShopTabHandler())
{
    m_shopView = new ShopView(parent);
    m_shopView->populateList(m_shopTabHandler->getBuyableItems());
}