#include "KnightDetailDialog.h"
#include "Player.h"
#include <QInputDialog>
#include <QMessageBox>

KnightDetailDialog::KnightDetailDialog(Knight &knight, QWidget *parent) : GameDialog(parent) {
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
    QString currentWeaponName = QString::fromStdString(knight.getRightHandWeapon().getName());
    QPushButton *weaponField = new QPushButton("Weapon: " + currentWeaponName, this);
    weaponField->setStyleSheet("padding: 15px; text-align: left; font-size: 14px; background: #1e1e1f; color: white;");
    mainLayout->addWidget(weaponField); 

    connect(weaponField, &QPushButton::clicked, this, [this, &knight, weaponField]() {
        std::vector<Item>& availableWeapons = Player::getInstance().getWeapons();

        if (availableWeapons.empty()) {
            // Fix QMessageBox frame too by using an instance
            QMessageBox alertBox(this);
            alertBox.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
            alertBox.setWindowTitle("Armoury Empty");
            alertBox.setText("You have no unassigned weapons in your inventory!");
            alertBox.exec();
            return;
        }

        QStringList options;
        for (const Item& item : availableWeapons) {
            options.append(QString::fromStdString(item.getName()));
        }

        QInputDialog dropDown(this);
        dropDown.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog); 
        dropDown.setWindowTitle("Select Weapon");
        dropDown.setLabelText("Choose a weapon to equip:");
        dropDown.setComboBoxItems(options);
        dropDown.setComboBoxEditable(false);

        // Execute and check response
        if (dropDown.exec() == QDialog::Accepted) {
            QString selectedName = dropDown.textValue();
            if (!selectedName.isEmpty()) {
                for (size_t i = 0; i < availableWeapons.size(); ++i) {
                    if (QString::fromStdString(availableWeapons[i].getName()) == selectedName) {
                        Item oldWeapon = knight.getRightHandWeapon();
                        if (oldWeapon.getName() != "None") { 
                            Player::getInstance().addWeapon(oldWeapon);
                        }
                        knight.equipRightHandWeapon(availableWeapons[i]);
                        availableWeapons.erase(availableWeapons.begin() + i);
                        weaponField->setText("Weapon: " + selectedName);
                        break;
                    }
                }
            }
        }
    });

    // --- ARMOUR FIELD BUTTON ---
    QString currentArmourName = QString::fromStdString(knight.getArmour().getName());
    QPushButton *armourField = new QPushButton("Armour: " + currentArmourName, this);
    armourField->setStyleSheet("padding: 15px; text-align: left; font-size: 14px; background: #1e1e1f; color: white;");
    mainLayout->addWidget(armourField);

    connect(armourField, &QPushButton::clicked, this, [this, &knight, armourField]() {
        std::vector<Item>& availableArmour = Player::getInstance().getArmour();

        if (availableArmour.empty()) {
            QMessageBox alertBox(this);
            alertBox.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
            alertBox.setWindowTitle("Armoury Empty");
            alertBox.setText("You have no unassigned armour in your inventory!");
            alertBox.exec();
            return;
        }

        QStringList options;
        for (const Item& item : availableArmour) {
            options.append(QString::fromStdString(item.getName()));
        }

        QInputDialog dropDown(this);
        dropDown.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
        dropDown.setWindowTitle("Select Armour");
        dropDown.setLabelText("Choose an armour to equip:");
        dropDown.setComboBoxItems(options);
        dropDown.setComboBoxEditable(false);

        if (dropDown.exec() == QDialog::Accepted) {
            QString selectedName = dropDown.textValue();
            if (!selectedName.isEmpty()) {
                for (size_t i = 0; i < availableArmour.size(); ++i) {
                    if (QString::fromStdString(availableArmour[i].getName()) == selectedName) {
                        Item oldWeapon = knight.getRightHandWeapon();
                        if (oldWeapon.getName() != "None") { 
                            Player::getInstance().addWeapon(oldWeapon);
                        }
                        knight.equipRightHandWeapon(availableArmour[i]);
                        availableArmour.erase(availableArmour.begin() + i);
                        armourField->setText("Armour: " + selectedName); // Fixed text indicator layout matching armour
                        break;
                    }
                }
            }
        }
    });

    mainLayout->addStretch();

    // Close Button
    QPushButton *closeButton = new QPushButton("Back to Roster", this);
    closeButton->setStyleSheet("padding: 10px; font-weight: bold;");
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    mainLayout->addWidget(closeButton);
}