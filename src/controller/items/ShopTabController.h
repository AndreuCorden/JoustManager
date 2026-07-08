#ifndef SHOPTABCONTROLLER_H
#define SHOPTABCONTROLLER_H

#pragma once

#include <QObject>
#include <QWidget>

#include "model/items/ShopTabHandler.h"
#include "view/items/ShopView.h"

class ShopTabController : public QObject
{
    Q_OBJECT
public:
    ShopTabController(QWidget *parent = nullptr);

    ShopView* getTab() { return m_shopView; }

    void startDay();

signals:
    void requestPayment(int amount, bool &approved);

private slots:
    void handleItemPurchase(const Item &item);

private:
    ShopView *m_shopView;
    ShopTabHandler *m_shopTabHandler;
};

#endif