#include "KnightDetailDialog.h"
#include "Player.h"
#include <QInputDialog>
#include <QMessageBox>

KnightDetailDialog::KnightDetailDialog(Knight &knight, QWidget *parent) : QDialog(parent) {
    // Set up window properties
    setWindowTitle(QString("%1's Profile").arg(QString::fromStdString(knight.getName())));
    setMinimumSize(350, 450);
    setModal(true); // Prevents interacting with the main menu until closed

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Header
    QLabel *nameLabel = new QLabel(QString::fromStdString(knight.getName()), this);
    nameLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #D4AF37;");
    layout->addWidget(nameLabel);

    // Display Stats (Example)
    QLabel *statsLabel = new QLabel("Stats & Equipment placeholder...", this);
    statsLabel->setStyleSheet("font-size: 14px; color: #4A5568;");
    layout->addWidget(statsLabel);

    QString currentWeaponName = QString::fromStdString(knight.getRightHandWeapon().getName());
    QPushButton *weaponField = new QPushButton("Weapon: " + currentWeaponName, this);
    weaponField->setStyleSheet("padding: 15px; text-align: left; font-size: 14px; background: #1e1e1f;");
    layout->addWidget(weaponField);

    connect(weaponField, &QPushButton::clicked, this, [this, &knight, weaponField]() {
        // Pull the available weapons from the global Player inventory
        std::vector<Item>& availableWeapons = Player::getInstance().getWeapons();

        if (availableWeapons.empty()) {
            QMessageBox::information(this, "Armoury Empty", "You have no unassigned weapons in your inventory!");
            return;
        }

        // Convert item names to QStringList for the Qt dropdown menu
        QStringList options;
        for (const Item& item : availableWeapons) {
            options.append(QString::fromStdString(item.getName()));
        }

        // Show the dropdown dialog to the user
        bool ok;
        QString selectedName = QInputDialog::getItem(this, "Select Weapon", 
                                                     "Choose a weapon to equip:", 
                                                     options, 0, false, &ok);

        if (ok && !selectedName.isEmpty()) {
            // Find the item index inside the player's inventory
            for (size_t i = 0; i < availableWeapons.size(); ++i) {
                if (QString::fromStdString(availableWeapons[i].getName()) == selectedName) {
                    
                    // Step A: If the knight already had a weapon, give it back to the player inventory
                    Item oldWeapon = knight.getRightHandWeapon();
                    if (oldWeapon.getName() != "None") { // Assuming "None" is your empty state
                        Player::getInstance().addWeapon(oldWeapon);
                    }

                    // Step B: Equip the new weapon onto the knight
                    knight.equipRightHandWeapon(availableWeapons[i]);

                    // Step C: Remove the item from the player's armory inventory
                    availableWeapons.erase(availableWeapons.begin() + i);

                    // Step D: Update the UI button text to reflect the change
                    weaponField->setText("Weapon: " + selectedName);
                    break;
                }
            }
        }
    });

    QString currentArmourName = QString::fromStdString(knight.getArmour().getName());
    QPushButton *armourField = new QPushButton("Armour: " + currentArmourName, this);
    armourField->setStyleSheet("padding: 15px; text-align: left; font-size: 14px; background: #1e1e1f;");
    layout->addWidget(armourField);

    // This one and the other one remove the weapon and armour from the list once selected. That's not correct. It should state who currently holds a weapon.
    connect(armourField, &QPushButton::clicked, this, [this, &knight, armourField]() {
        // Pull the available weapons from the global Player inventory
        std::vector<Item>& availableArmour = Player::getInstance().getArmour();

        if (availableArmour.empty()) {
            QMessageBox::information(this, "Armoury Empty", "You have no unassigned armour in your inventory!");
            return;
        }

        // Convert item names to QStringList for the Qt dropdown menu
        QStringList options;
        for (const Item& item : availableArmour) {
            options.append(QString::fromStdString(item.getName()));
        }

        // Show the dropdown dialog to the user
        bool ok;
        QString selectedName = QInputDialog::getItem(this, "Select Armour", 
                                                     "Choose an armour to equip:", 
                                                     options, 0, false, &ok);

        if (ok && !selectedName.isEmpty()) {
            // Find the item index inside the player's inventory
            for (size_t i = 0; i < availableArmour.size(); ++i) {
                if (QString::fromStdString(availableArmour[i].getName()) == selectedName) {
                    
                    // Step A: If the knight already had a weapon, give it back to the player inventory
                    Item oldWeapon = knight.getRightHandWeapon();
                    if (oldWeapon.getName() != "None") { // Assuming "None" is your empty state
                        Player::getInstance().addWeapon(oldWeapon);
                    }

                    // Step B: Equip the new weapon onto the knight
                    knight.equipRightHandWeapon(availableArmour[i]);

                    // Step C: Remove the item from the player's armory inventory
                    availableArmour.erase(availableArmour.begin() + i);

                    // Step D: Update the UI button text to reflect the change
                    armourField->setText("Weapon: " + selectedName);
                    break;
                }
            }
        }
    });

    // Add spacing
    layout->addStretch();

    // Close Button
    QPushButton *closeButton = new QPushButton("Back to Roster", this);
    closeButton->setStyleSheet("padding: 10px; font-weight: bold;");
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(closeButton);

    setLayout(layout);
}