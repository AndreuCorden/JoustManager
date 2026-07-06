#ifndef SHOPTABCONTROLLER_H
#define SHOPTABCONTROLLER_H

#pragma once

#include <QObject>
#include <QWidget>

#include "Items/ShopTabHandler.h"
#include "Items/ShopView.h"

class ShopTabController : public QObject
{
    Q_OBJECT
public:
    ShopTabController(QWidget *parent = nullptr);

    ShopView* getTab() { return m_shopView; }

private slots:
    void handleItemPurchase(const Item &item);

private:
    ShopView *m_shopView;
    ShopTabHandler *m_shopTabHandler;
};

#endif