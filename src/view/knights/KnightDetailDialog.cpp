#include "view/knights/KnightDetailDialog.h"
#include "Player.h"
#include "model/items/Item.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QTabWidget>
#include <QFormLayout>
#include <QProgressBar>

// Helper function to build clean, stylized skill readouts
QWidget* createStatRow(const QString &statName, int value) {
    QWidget *container = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->setContentsMargins(10, 5, 10, 5);

    QLabel *label = new QLabel(statName, container);
    label->setStyleSheet("font-weight: bold; color: #CBD5E1;");
    
    QProgressBar *bar = new QProgressBar(container);
    bar->setRange(0, 100);
    bar->setValue(value);
    bar->setTextVisible(true);
    bar->setFormat("%v / 100");
    bar->setStyleSheet(
        "QProgressBar { background-color: #374151; border: 1px solid #4B5563; border-radius: 4px; text-align: center; color: white; font-weight: bold; height: 20px; }"
        "QProgressBar::chunk { background-color: #D4AF37; border-radius: 3px; }"
    );

    layout->addWidget(label, 2);
    layout->addWidget(bar, 3);
    return container;
}

QWidget *KnightDetailDialog::createEquipmentSlot(Knight &knight, Item::ItemType type)
{
    QWidget *rowContainer = new QWidget(this);
    QHBoxLayout *rowLayout = new QHBoxLayout(rowContainer);
    rowLayout->setContentsMargins(0, 0, 0, 0);
    rowLayout->setSpacing(8);

    std::string labelPrefix = "Body";
    if (type == Item::ItemType::Weapon) labelPrefix = "Right Hand";
    // Add additional handling here if you expand your ItemType enum for Left Hand/Shields later!

    Item currentItem = (type == Item::ItemType::Weapon) ? knight.getRightHandWeapon() : knight.getArmour();
    QString currentItemName = QString::fromStdString(currentItem.getName());

    QPushButton *field = new QPushButton(QString::fromStdString(labelPrefix) + ": " + currentItemName, rowContainer);
    field->setStyleSheet("padding: 15px; text-align: left; font-size: 14px; background: #1E293B; color: white; border: 1px solid #475569; border-radius: 4px;");

    QPushButton *unequipBtn = new QPushButton("❌", rowContainer);
    unequipBtn->setFixedWidth(45);

    QString activeRedStyle = "padding: 15px; font-size: 14px; background: #991B1B; color: white; font-weight: bold; border-radius: 4px;";
    QString disabledGrayStyle = "padding: 15px; font-size: 14px; background: #334155; color: #64748B; border-radius: 4px;";

    bool hasItemEquipped = (currentItemName != "None" && !currentItemName.isEmpty());
    unequipBtn->setEnabled(hasItemEquipped);
    unequipBtn->setStyleSheet(hasItemEquipped ? activeRedStyle : disabledGrayStyle);

    connect(unequipBtn, &QPushButton::clicked, this, [this, &knight, field, unequipBtn, type, labelPrefix, disabledGrayStyle]() {
        Item oldItem = (type == Item::ItemType::Weapon) ? knight.getRightHandWeapon() : knight.getArmour();
        if (oldItem.getName() != "None") { 
            if (type == Item::ItemType::Weapon) {
                m_player.addWeapon(oldItem);
                knight.equipRightHandWeapon(Item());
            } else {
                m_player.addArmour(oldItem);
                knight.equipArmour(Item());
            }
        }
        field->setText(QString::fromStdString(labelPrefix) + ": None");
        unequipBtn->setEnabled(false);
        unequipBtn->setStyleSheet(disabledGrayStyle);
    });

    connect(field, &QPushButton::clicked, this, [this, &knight, field, unequipBtn, type, labelPrefix, activeRedStyle]() {
        std::vector<Item>& availableItems = (type == Item::ItemType::Weapon) ? m_player.getWeapons() : m_player.getArmour();
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
            alertBox.setText(QString("No unassigned items in your armoury vault!"));
            alertBox.exec();
            return;
        }

        QInputDialog dropDown(this);
        dropDown.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog); 
        dropDown.setComboBoxItems(options);
        dropDown.setComboBoxEditable(false);

        if (dropDown.exec() == QDialog::Accepted) {
            QString selectedName = dropDown.textValue();
            if (!selectedName.isEmpty()) {
                for (size_t i = 0; i < availableItems.size(); ++i) {
                    if (QString::fromStdString(availableItems[i].getName()) == selectedName) {
                        Item oldItem = (type == Item::ItemType::Weapon) ? knight.getRightHandWeapon() : knight.getArmour();
                        if (oldItem.getName() != "None") { 
                            if (type == Item::ItemType::Weapon) m_player.addWeapon(oldItem);
                            else m_player.addArmour(oldItem);
                        }
                        
                        if (type == Item::ItemType::Weapon) knight.equipRightHandWeapon(availableItems[i]);
                        else knight.equipArmour(availableItems[i]); 
                        
                        availableItems.erase(availableItems.begin() + i);
                        field->setText(QString::fromStdString(labelPrefix) + ": " + selectedName);
                        unequipBtn->setEnabled(true);
                        unequipBtn->setStyleSheet(activeRedStyle);
                        break;
                    }
                }
            }
        }
    });

    rowLayout->addWidget(field, 1);
    rowLayout->addWidget(unequipBtn);
    return rowContainer;
}

// ============================================================================
// 1. ROSTER VIEW CONSTRUCTOR (EDITABLE EQUIPMENT)
// ============================================================================
KnightDetailDialog::KnightDetailDialog(Player &player, Knight &knight, KnightRosterView *parent)
: GameDialog(parent), m_player(player)
{
    setMinimumSize(500, 550);
    setModal(true);

    QLabel *nameLabel = new QLabel(QString::fromStdString(knight.getName()), this);
    nameLabel->setStyleSheet("font-size: 26px; font-weight: bold; color: #D4AF37; margin-bottom: 10px;");
    mainLayout->addWidget(nameLabel);

    QTabWidget *tabs = new QTabWidget(this);

    // --- TAB 1: PROFILE ---
    QWidget *profileTab = new QWidget();
    QFormLayout *profileLayout = new QFormLayout(profileTab);
    profileLayout->addRow("Title:", new QLabel(QString::fromStdString(knight.getTitle())));
    profileLayout->addRow("Origin:", new QLabel(QString::fromStdString(knight.getOrigin())));
    profileLayout->addRow("Level:", new QLabel(QString::number(knight.getLevel())));
    profileLayout->addRow("Height:", new QLabel(QString("%1 cm").arg(knight.getHeight())));
    profileLayout->addRow("Weight:", new QLabel(QString("%1 kg").arg(knight.getWeight())));
    tabs->addTab(profileTab, "📋 Profile");

    // --- TAB 2: EQUIPMENT (INTERACTIVE) ---
    QWidget *equipTab = new QWidget();
    QVBoxLayout *equipLayout = new QVBoxLayout(equipTab);
    equipLayout->setSpacing(12);
    equipLayout->addWidget(new QLabel("<b>Equipped Gear:</b>"));
    equipLayout->addWidget(createEquipmentSlot(knight, Item::ItemType::Armour));   // Body
    equipLayout->addWidget(createEquipmentSlot(knight, Item::ItemType::Weapon));   // Right Hand
    
    // Left Hand Placeholder (until you add Shield types, showing read-only empty slot)
    QLabel *leftHandLabel = new QLabel("Left Hand: Empty (Shield Slot)", this);
    leftHandLabel->setStyleSheet("padding: 15px; background: #111827; border-radius: 4px; color: #64748B;");
    equipLayout->addWidget(leftHandLabel);
    equipLayout->addStretch();
    tabs->addTab(equipTab, "🛡️ Equipment");

    // --- TAB 3: JOUSTING ---
    QWidget *joustTab = new QWidget();
    QVBoxLayout *joustLayout = new QVBoxLayout(joustTab);
    joustLayout->addWidget(createStatRow("Horsemanship", knight.getHorsemanship()));
    joustLayout->addWidget(createStatRow("Lance Precision", knight.getLancePrecision()));
    joustLayout->addWidget(createStatRow("Bracing", knight.getPosie()));
    joustLayout->addStretch();
    tabs->addTab(joustTab, "🏇 Jousting");

    // --- TAB 4: SWORDFIGHTING ---
    QWidget *swordTab = new QWidget();
    QVBoxLayout *swordLayout = new QVBoxLayout(swordTab);
    swordLayout->addWidget(createStatRow("Weapon Mastery", knight.getSwordplay()));
    swordLayout->addWidget(createStatRow("Footwork", knight.getFootwork()));
    swordLayout->addWidget(createStatRow("Vigor", knight.getVigor()));
    swordLayout->addStretch();
    tabs->addTab(swordTab, "⚔️ Melee");

    // --- TAB 5: ARCHERY ---
    QWidget *archeryTab = new QWidget();
    QVBoxLayout *archeryLayout = new QVBoxLayout(archeryTab);
    archeryLayout->addWidget(createStatRow("Marksmanship", knight.getMarksmanship()));
    archeryLayout->addWidget(createStatRow("Composure", knight.getFocus()));
    archeryLayout->addStretch();
    tabs->addTab(archeryTab, "🎯 Archery");

    mainLayout->addWidget(tabs);

    QPushButton *closeButton = new QPushButton("Back to Roster", this);
    closeButton->setStyleSheet("padding: 12px; font-weight: bold; background-color: #374151; color: white; border-radius: 4px; margin-top: 10px;");
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    mainLayout->addWidget(closeButton);
}

// ============================================================================
// 2. RECRUITMENT VIEW CONSTRUCTOR (READ-ONLY EQUIPMENT)
// ============================================================================
KnightDetailDialog::KnightDetailDialog(Player &player, const Knight &knight, KnightRecruitmentView *parent) 
: GameDialog(parent), m_player(player)
{
    setMinimumSize(500, 550);
    setModal(true);

    QLabel *nameLabel = new QLabel(QString::fromStdString(knight.getName()), this);
    nameLabel->setStyleSheet("font-size: 26px; font-weight: bold; color: #D4AF37; margin-bottom: 10px;");
    mainLayout->addWidget(nameLabel);

    QTabWidget *tabs = new QTabWidget(this);

    // --- TAB 1: PROFILE ---
    QWidget *profileTab = new QWidget();
    QFormLayout *profileLayout = new QFormLayout(profileTab);
    profileLayout->addRow("Title:", new QLabel(QString::fromStdString(knight.getTitle())));
    profileLayout->addRow("Origin:", new QLabel(QString::fromStdString(knight.getOrigin())));
    profileLayout->addRow("Level:", new QLabel(QString::number(knight.getLevel())));
    profileLayout->addRow("Height:", new QLabel(QString("%1 cm").arg(knight.getHeight())));
    profileLayout->addRow("Weight:", new QLabel(QString("%1 kg").arg(knight.getWeight())));
    tabs->addTab(profileTab, "📋 Profile");

    // --- TAB 2: EQUIPMENT (READ ONLY FOR LUCK SELLING) ---
    QWidget *equipTab = new QWidget();
    QVBoxLayout *equipLayout = new QVBoxLayout(equipTab);
    equipLayout->setSpacing(8);
    equipLayout->addWidget(new QLabel("<span style='color: #FBBF24;'>Hire this knight to change their equipment:</span>"));
    
    QString bodyGear = QString("Body: %1").arg(QString::fromStdString(knight.getArmour().getName()));
    QString weaponGear = QString("Right Hand: %1").arg(QString::fromStdString(knight.getRightHandWeapon().getName()));

    QLabel *bodyLabel = new QLabel(bodyGear, this);
    bodyLabel->setStyleSheet("padding: 12px; background: #1E293B; border-radius: 4px; border-left: 4px solid #64748B;");
    QLabel *weaponLabel = new QLabel(weaponGear, this);
    weaponLabel->setStyleSheet("padding: 12px; background: #1E293B; border-radius: 4px; border-left: 4px solid #64748B;");
    QLabel *leftLabel = new QLabel("Left Hand: None", this);
    leftLabel->setStyleSheet("padding: 12px; background: #1E293B; border-radius: 4px; border-left: 4px solid #64748B; color: #64748B;");

    equipLayout->addWidget(bodyLabel);
    equipLayout->addWidget(weaponLabel);
    equipLayout->addWidget(leftLabel);
    equipLayout->addStretch();
    tabs->addTab(equipTab, "🛡️ Equipment");

    // --- TAB 3: JOUSTING ---
    QWidget *joustTab = new QWidget();
    QVBoxLayout *joustLayout = new QVBoxLayout(joustTab);
    joustLayout->addWidget(createStatRow("Horsemanship", knight.getHorsemanship()));
    joustLayout->addWidget(createStatRow("Lance Precision", knight.getLancePrecision()));
    joustLayout->addWidget(createStatRow("Bracing", knight.getPosie()));
    joustLayout->addStretch();
    tabs->addTab(joustTab, "🏇 Jousting");

    // --- TAB 4: SWORDFIGHTING ---
    QWidget *swordTab = new QWidget();
    QVBoxLayout *swordLayout = new QVBoxLayout(swordTab);
    swordLayout->addWidget(createStatRow("Weapon Mastery", knight.getSwordplay()));
    swordLayout->addWidget(createStatRow("Footwork", knight.getFootwork()));
    swordLayout->addWidget(createStatRow("Vigor", knight.getVigor()));
    swordLayout->addStretch();
    tabs->addTab(swordTab, "⚔️ Melee");

    // --- TAB 5: ARCHERY ---
    QWidget *archeryTab = new QWidget();
    QVBoxLayout *archeryLayout = new QVBoxLayout(archeryTab);
    archeryLayout->addWidget(createStatRow("Marksmanship", knight.getMarksmanship()));
    archeryLayout->addWidget(createStatRow("Composure", knight.getFocus()));
    archeryLayout->addStretch();
    tabs->addTab(archeryTab, "🎯 Archery");

    mainLayout->addWidget(tabs);

    // Bottom Actions
    QHBoxLayout *actionsLayout = new QHBoxLayout();
    
    QPushButton *buyButton = new QPushButton("⚔️ Hire Knight", this);
    buyButton->setStyleSheet("padding: 12px; font-weight: bold; background-color: #059669; color: white; border-radius: 4px;");
    connect(buyButton, &QPushButton::clicked, this, [this]() { this->accept(); });
    
    QPushButton *closeButton = new QPushButton("Cancel", this);
    closeButton->setStyleSheet("padding: 12px; font-weight: bold; background-color: #4B5563; color: white; border-radius: 4px;");
    connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);
    
    actionsLayout->addWidget(buyButton, 2);
    actionsLayout->addWidget(closeButton, 1);
    mainLayout->addLayout(actionsLayout);
}