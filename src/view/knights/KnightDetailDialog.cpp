#include "view/knights/KnightDetailDialog.h"
#include "Player.h"
#include "model/items/Item.h"
#include <QInputDialog>
#include <QMessageBox>

QWidget *KnightDetailDialog::createEquipmentSlot(Knight &knight, Item::ItemType type)
{
    // 1. Setup a container widget and horizontal row layout
    QWidget *rowContainer = new QWidget(this);
    QHBoxLayout *rowLayout = new QHBoxLayout(rowContainer);
    rowLayout->setContentsMargins(0, 0, 0, 0);
    rowLayout->setSpacing(8);

    std::string labelPrefix = (type == Item::ItemType::Weapon) ? "Weapon" : "Armour";
    Item currentItem = (type == Item::ItemType::Weapon) ? knight.getRightHandWeapon() : knight.getArmour();
    QString currentItemName = QString::fromStdString(currentItem.getName());

    // 2. Create the main Item Selection Button
    QPushButton *field = new QPushButton(QString::fromStdString(labelPrefix) + ": " + currentItemName, rowContainer);
    field->setStyleSheet("padding: 15px; text-align: left; font-size: 14px; background: #1e1e1f; color: white; border-radius: 4px;");

    // 3. Create the dedicated Unequip Button
    QPushButton *unequipBtn = new QPushButton("❌", rowContainer);
    unequipBtn->setFixedWidth(45);

    // Style sheets for enabled/disabled states
    QString activeRedStyle = "padding: 15px; font-size: 14px; background: #991B1B; color: white; font-weight: bold; border-radius: 4px;";
    QString disabledGrayStyle = "padding: 15px; font-size: 14px; background: #2D3748; color: #718096; border-radius: 4px;";

    // Determine if an item is actively equipped to toggle initial button state
    bool hasItemEquipped = (currentItemName != "None" && !currentItemName.isEmpty());
    unequipBtn->setEnabled(hasItemEquipped);
    unequipBtn->setStyleSheet(hasItemEquipped ? activeRedStyle : disabledGrayStyle);

    // --- CONNECTION 1: EXPLICIT UNEQUIP CLICK ---
    connect(unequipBtn, &QPushButton::clicked, this, [this, &knight, field, unequipBtn, type, labelPrefix, disabledGrayStyle]()
            {
        Item oldItem = (type == Item::ItemType::Weapon) ? knight.getRightHandWeapon() : knight.getArmour();
        
        // Return item back to the armory vault safely
        if (oldItem.getName() != "None") { 
            if (type == Item::ItemType::Weapon) {
                Player::getInstance().addWeapon(oldItem);
                knight.equipRightHandWeapon(Item()); // Equips a blank/default item constructor
            } else {
                Player::getInstance().addArmour(oldItem);
                knight.equipArmour(Item()); // Equips a blank/default item constructor
            }
        }

        // Wipe the UI representation instantly
        field->setText(QString::fromStdString(labelPrefix) + ": None");
        unequipBtn->setEnabled(false);
        unequipBtn->setStyleSheet(disabledGrayStyle); });

    // --- CONNECTION 2: ITEM SELECTION DROPDOWN (Modified from your original) ---
    connect(field, &QPushButton::clicked, this, [this, &knight, field, unequipBtn, type, labelPrefix, activeRedStyle]()
            {
        std::vector<Item>& availableItems = (type == Item::ItemType::Weapon) 
            ? Player::getInstance().getWeapons() 
            : Player::getInstance().getArmour();

        QStringList options;
        for (const Item& item : availableItems) {
            std::string name = item.getName();
            if (name != "None" && !name.empty()) {
                options.append(QString::fromStdString(name));
            }
        }

        if (options.isEmpty()) {
            QMessageBox alertBox(this);
            alertBox.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
            alertBox.setWindowTitle("Armoury Empty");
            alertBox.setText(QString("You have no unassigned %1 items in your inventory!").arg(QString::fromStdString(labelPrefix).toLower()));
            alertBox.exec();
            return;
        }

        QInputDialog dropDown(this);
        dropDown.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog); 
        dropDown.setWindowTitle("Select " + QString::fromStdString(labelPrefix));
        dropDown.setLabelText(QString("Choose %1 to equip:").arg(QString::fromStdString(labelPrefix).toLower()));
        dropDown.setComboBoxItems(options);
        dropDown.setComboBoxEditable(false);

        if (dropDown.exec() == QDialog::Accepted) {
            QString selectedName = dropDown.textValue();
            if (!selectedName.isEmpty()) {
                for (size_t i = 0; i < availableItems.size(); ++i) {
                    if (QString::fromStdString(availableItems[i].getName()) == selectedName) {
                        
                        // Extract and return old equipment if applicable
                        Item oldItem = (type == Item::ItemType::Weapon) ? knight.getRightHandWeapon() : knight.getArmour();
                        if (oldItem.getName() != "None") { 
                            if (type == Item::ItemType::Weapon) {
                                Player::getInstance().addWeapon(oldItem);
                            } else {
                                Player::getInstance().addArmour(oldItem); 
                            }
                        }
                        
                        // Bind new item
                        if (type == Item::ItemType::Weapon) {
                            knight.equipRightHandWeapon(availableItems[i]);
                        } else {
                            knight.equipArmour(availableItems[i]); 
                        }
                        
                        availableItems.erase(availableItems.begin() + i);
                        field->setText(QString::fromStdString(labelPrefix) + ": " + selectedName);
                        
                        // 🌟 Crucial UI Sync: Enable the clear button since an item is active!
                        unequipBtn->setEnabled(true);
                        unequipBtn->setStyleSheet(activeRedStyle);
                        break;
                    }
                }
            }
        } });

    // Add widgets to our row layout tracking line
    rowLayout->addWidget(field, 1); // Give the item button a stretch factor of 1 to span full width
    rowLayout->addWidget(unequipBtn);

    return rowContainer;
}

KnightDetailDialog::KnightDetailDialog(Knight &knight, KnightRosterView *parent) : GameDialog(parent)
{
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

KnightDetailDialog::KnightDetailDialog(const Knight &knight, KnightRecruitmentView *parent) : GameDialog(parent)
{
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
    QPushButton *buyButton = new QPushButton("Buy Knight", this);
    buyButton->setStyleSheet("padding: 10px; font-weight: bold;");
    connect(buyButton, &QPushButton::clicked, this, [this, &knight]()
            { this->accept(); });
    mainLayout->addWidget(buyButton);

    // Close Button
    QPushButton *closeButton = new QPushButton("Back to Roster", this);
    closeButton->setStyleSheet("padding: 10px; font-weight: bold;");
    connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);
    mainLayout->addWidget(closeButton);
}