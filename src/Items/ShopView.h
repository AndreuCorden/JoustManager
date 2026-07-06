#ifndef SHOPVIEW_H
#define SHOPVIEW_H

#pragma once

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "Items/Item.h"

#include <list>

class ShopView : public QWidget
{
    Q_OBJECT
public:
    explicit ShopView(QWidget *parent = nullptr);

    void populateList(const std::list<Item> &buyableItems);

private:
    QVBoxLayout *listLayout;
};

#endif