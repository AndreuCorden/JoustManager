#ifndef ITEMDETAILDIALOG_H
#define ITEMDETAILDIALOG_H

#pragma once
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include "model/items/Item.h"
#include "view/items/ShopView.h"
#include "GameDialog.h"


class ItemDetailDialog : public GameDialog {
    Q_OBJECT

public:
    // Pass the knight by reference so modifications (like changing gear) save back to the roster
    ItemDetailDialog(const Item &item, ShopView *parent = nullptr);

private:
};

#endif