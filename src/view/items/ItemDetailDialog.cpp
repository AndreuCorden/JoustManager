#include "view/items/ItemDetailDialog.h"
#include "Player.h"
#include "model/items/Item.h"
#include <QInputDialog>
#include <QMessageBox>

ItemDetailDialog::ItemDetailDialog(const Item &item, ShopView *parent) : GameDialog(parent) {
    // Set up window properties
    setWindowTitle(QString("%1's Profile").arg(QString::fromStdString(item.getName())));
    setMinimumSize(350, 450);
    setModal(true); 

    // Header
    QLabel *nameLabel = new QLabel(QString::fromStdString(item.getName()), this);
    nameLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #D4AF37;");
    mainLayout->addWidget(nameLabel);

    // Display Stats (Example)
    QLabel *statsLabel = new QLabel("Stats placeholder...", this);
    statsLabel->setStyleSheet("font-size: 14px; color: #4A5568;");
    mainLayout->addWidget(statsLabel);

    mainLayout->addStretch();

    // Buy Button
    QPushButton *buyButton = new QPushButton("Buy Item",this);
    buyButton->setStyleSheet("padding: 10px; font-weight: bold;");
    connect(buyButton, &QPushButton::clicked, this, [this, &item]()
    {
        if(item.getItemType() == Item::ItemType::Armour)
        {
            Player::getInstance().addArmour(item);
        }
        else
        {
            Player::getInstance().addWeapon(item);
        }
        
        Player::getInstance().modifyGold(-item.getCost());

        this->accept();
    });
    mainLayout->addWidget(buyButton);

    // Close Button
    QPushButton *closeButton = new QPushButton("Back to Roster", this);
    closeButton->setStyleSheet("padding: 10px; font-weight: bold;");
    connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);
    mainLayout->addWidget(closeButton);
}