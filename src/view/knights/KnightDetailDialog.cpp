#include "view/knights/KnightDetailDialog.h"
#include "Player.h"
#include "model/items/Item.h"
#include <QInputDialog>
#include <QMessageBox>

QPushButton* KnightDetailDialog::createEquipmentSlot(Knight &knight, Item::ItemType type)
{
    // 1. Resolve configuration values based on the enum type
    std::string labelPrefix = (type == Item::ItemType::Weapon) ? "Weapon" : "Armour";
    Item currentItem       = (type == Item::ItemType::Weapon) ? knight.getRightHandWeapon() : knight.getArmour();

    QString currentItemName = QString::fromStdString(currentItem.getName());
    QPushButton *field = new QPushButton(QString::fromStdString(labelPrefix) + ": " + currentItemName, this);
    field->setStyleSheet("padding: 15px; text-align: left; font-size: 14px; background: #1e1e1f; color: white;");

    connect(field, &QPushButton::clicked, this, [this, &knight, field, type, labelPrefix]() {
        
        // 2. Point to the correct live inventory vector
        std::vector<Item>& availableItems = (type == Item::ItemType::Weapon) 
            ? Player::getInstance().getWeapons() 
            : Player::getInstance().getArmour();

        if (availableItems.empty()) {
            QMessageBox alertBox(this);
            alertBox.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
            alertBox.setWindowTitle("Armoury Empty");
            alertBox.setText(QString("You have no unassigned %1 items in your inventory!")
                             .arg(QString::fromStdString(labelPrefix).toLower()));
            alertBox.exec();
            return;
        }

        QStringList options;
        for (const Item& item : availableItems) {
            options.append(QString::fromStdString(item.getName()));
        }

        QInputDialog dropDown(this);
        dropDown.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog); 
        dropDown.setWindowTitle("Select " + QString::fromStdString(labelPrefix));
        dropDown.setLabelText(QString("Choose %1 to equip:")
                              .arg(QString::fromStdString(labelPrefix).toLower()));
        dropDown.setComboBoxItems(options);
        dropDown.setComboBoxEditable(false);

        if (dropDown.exec() == QDialog::Accepted) {
            QString selectedName = dropDown.textValue();
            if (!selectedName.isEmpty()) {
                for (size_t i = 0; i < availableItems.size(); ++i) {
                    if (QString::fromStdString(availableItems[i].getName()) == selectedName) {
                        
                        // 3. Extract and return old equipment safely (Bug-free!)
                        Item oldItem = (type == Item::ItemType::Weapon) ? knight.getRightHandWeapon() : knight.getArmour();
                        if (oldItem.getName() != "None") { 
                            if (type == Item::ItemType::Weapon) {
                                Player::getInstance().addWeapon(oldItem);
                            } else {
                                Player::getInstance().addArmour(oldItem); 
                            }
                        }
                        
                        // 4. Bind the new item to the correct entity property
                        if (type == Item::ItemType::Weapon) {
                            knight.equipRightHandWeapon(availableItems[i]);
                        } else {
                            knight.equipArmour(availableItems[i]); 
                        }
                        
                        // 5. Erase from vault and refresh UI text
                        availableItems.erase(availableItems.begin() + i);
                        field->setText(QString::fromStdString(labelPrefix) + ": " + selectedName);
                        break;
                    }
                }
            }
        }
    });

    return field;
}

KnightDetailDialog::KnightDetailDialog(Knight &knight, KnightRosterView *parent) : GameDialog(parent) {
    // Set up window properties
    setWindowTitle(QString("%1's Profile").arg(QString::fromStdString(knight.getName())));
    setMinimumSize(350, 450);
    setModal(true); 

    // Header
    QLabel *nameLabel = new QLabel(QString::fromStdString(knight.getName()), this);
    nameLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #D4AF37;");
    mainLayout->addWidget(nameLabel);

    // Display Stats (Example)
    QLabel *statsLabel = new QLabel("Stats & Equipment placeholder...", this);
    statsLabel->setStyleSheet("font-size: 14px; color: #4A5568;");
    mainLayout->addWidget(statsLabel);

    // --- WEAPON FIELD BUTTON ---
    
    mainLayout->addWidget(createEquipmentSlot(knight, Item::ItemType::Weapon)); 

    // --- ARMOUR FIELD BUTTON ---
    mainLayout->addWidget(createEquipmentSlot(knight, Item::ItemType::Armour));
    mainLayout->addStretch();

    // Close Button
    QPushButton *closeButton = new QPushButton("Back to Roster", this);
    closeButton->setStyleSheet("padding: 10px; font-weight: bold;");
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    mainLayout->addWidget(closeButton);
}

KnightDetailDialog::KnightDetailDialog(const Knight &knight, KnightRecruitmentView *parent) : GameDialog(parent) {
    // Set up window properties
    setWindowTitle(QString("%1's Profile").arg(QString::fromStdString(knight.getName())));
    setMinimumSize(350, 450);
    setModal(true); 

    // Header
    QLabel *nameLabel = new QLabel(QString::fromStdString(knight.getName()), this);
    nameLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #D4AF37;");
    mainLayout->addWidget(nameLabel);

    // Display Stats (Example)
    QLabel *statsLabel = new QLabel("Stats & Equipment placeholder...", this);
    statsLabel->setStyleSheet("font-size: 14px; color: #4A5568;");
    mainLayout->addWidget(statsLabel);

    mainLayout->addStretch();

    // Buy Button
    QPushButton *buyButton = new QPushButton("Buy Knight",this);
    buyButton->setStyleSheet("padding: 10px; font-weight: bold;");
    connect(buyButton, &QPushButton::clicked, this, [this, &knight]()
    {
        Player::getInstance().addKnight(knight);
        Player::getInstance().modifyGold(-knight.getCost());

        this->accept();
    });
    mainLayout->addWidget(buyButton);

    // Close Button
    QPushButton *closeButton = new QPushButton("Back to Roster", this);
    closeButton->setStyleSheet("padding: 10px; font-weight: bold;");
    connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);
    mainLayout->addWidget(closeButton);
}