#ifndef SHOPTABCONTROLLER_H
#define SHOPTABCONTROLLER_H

#pragma once

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "Items/ShopTabHandler.h"
#include "Items/ShopView.h"

class ShopTabController
{
public:
    ShopTabController(QWidget *parent = nullptr);

    ShopView* getTab() { return m_shopView; }

private:
    ShopView *m_shopView;
    ShopTabHandler *m_shopTabHandler;
};

#endif