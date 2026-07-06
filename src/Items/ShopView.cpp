#include "Items/ShopView.h"
#include "Items/Item.h"

ShopView::ShopView(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *label = new QLabel("ARMOURY & BLACKSMITH", this);
    label->setStyleSheet("font-size: 20px; font-weight: bold; color: #4A5568;");
    mainLayout->addWidget(label);

    // 🌟 FIX 2: Allocate memory for your dynamic item list layout container!
    listLayout = new QVBoxLayout();
    mainLayout->addLayout(listLayout);

    mainLayout->addStretch();
}

void ShopView::populateList(const std::list<Item> &buyableItems)
{
    // 1. CLEANUP FLUSH: Wipe out any old buttons currently on screen
    QLayoutItem *child;
    while ((child = listLayout->takeAt(0)) != nullptr)
    {
        if (child->widget())
        {
            child->widget()->deleteLater();
        }
        delete child;
    }

    // 2. REBUILD ROWS using iterators instead of indices
    // We use a regular loop but keep tracking the live iterator 'it'
    for (const Item &i : buyableItems)
    {

        QString buttonText = QString("%1 (See Stats)").arg(QString::fromStdString(i.getName()));
        QPushButton *itemButton = new QPushButton(buttonText, this);
        itemButton->setStyleSheet("padding: 12px; font-size: 14px;");
        listLayout->addWidget(itemButton);

        /*connect(itemButton, &QPushButton::clicked, this, [this, index]()
                { emit itemSelected(index); });*/
    }
}