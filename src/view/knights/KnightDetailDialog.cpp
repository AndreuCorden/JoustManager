#include "view/knights/KnightDetailDialog.h"
#include "Player.h"
#include "model/items/Item.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QTabWidget>
#include <QFormLayout>
#include <QProgressBar>
#include <QGridLayout>
#include <QScrollArea>

#include <vector>
#include <functional>

// Helper function to build clean, stylized skill progress rows
QWidget* createStatRow(const QString &statName, int value) {
    QWidget *container = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->setContentsMargins(5, 4, 5, 4);

    QLabel *label = new QLabel(statName, container);
    label->setStyleSheet("font-weight: bold; color: #CBD5E1; font-size: 13px;");
    
    QProgressBar *bar = new QProgressBar(container);
    bar->setRange(0, 100);
    bar->setValue(value);
    bar->setTextVisible(true);
    bar->setFormat("%v/100");
    bar->setStyleSheet(
        "QProgressBar { background-color: #2D3748; border: 1px solid #4A5568; border-radius: 4px; text-align: center; color: white; font-weight: bold; height: 24px; font-size: 11px; }"
        "QProgressBar::chunk { background-color: #D4AF37; border-radius: 2px; }"
    );

    layout->addWidget(label, 2);
    layout->addWidget(bar, 3);
    layout->addSpacing(28); // Matches the width of the editable "+" button column spacing
    return container;
}

// Helper to build section headers inside the combined combat tab
QWidget* createSectionHeader(const QString &title, const QString &icon) {
    QLabel *header = new QLabel(QString("%1  %2").arg(icon, title));
    header->setStyleSheet("font-family: 'Georgia', serif; font-size: 14px; font-weight: bold; color: #D4AF37; margin-top: 5px; border-bottom: 1px solid #4A5568; padding-bottom: 3px;");
    return header;
}

QWidget *KnightDetailDialog::createEquipmentSlot(Knight &knight, Item::ItemType type)
{
    QWidget *rowContainer = new QWidget(this);
    QHBoxLayout *rowLayout = new QHBoxLayout(rowContainer);
    rowLayout->setContentsMargins(0, 0, 0, 0);
    rowLayout->setSpacing(8);

    std::string labelPrefix = "Body";
    if (type == Item::ItemType::Weapon) labelPrefix = "Right Hand";

    Item currentItem = (type == Item::ItemType::Weapon) ? knight.getRightHandWeapon() : knight.getArmour();
    QString currentItemName = QString::fromStdString(currentItem.getName());

    QPushButton *field = new QPushButton(QString::fromStdString(labelPrefix) + ": " + currentItemName, rowContainer);
    field->setStyleSheet("padding: 12px; text-align: left; font-size: 13px; background: #2D3748; color: white; border: 1px solid #4A5568; border-radius: 4px;");

    QPushButton *unequipBtn = new QPushButton("❌", rowContainer);
    unequipBtn->setFixedWidth(40);

    QString activeRedStyle = "padding: 12px; font-size: 13px; background: #991B1B; color: white; font-weight: bold; border-radius: 4px;";
    QString disabledGrayStyle = "padding: 12px; font-size: 13px; background: #1A202C; color: #4A5568; border: 1px solid #2D3748; border-radius: 4px;";

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

// Helper to create a dynamic profile view card layout
QWidget* createDynamicProfileCard(const Knight &knight) {
    QWidget *container = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(15);

    // 1. TOP HERO CARD ROW
    QHBoxLayout *topCardLayout = new QHBoxLayout();
    topCardLayout->setSpacing(15);

    // Left Column: Level/Crest Badge
    QLabel *crestBadge = new QLabel(QString("LVL\n%1").arg(knight.getLevel()));
    crestBadge->setAlignment(Qt::AlignCenter);
    crestBadge->setStyleSheet(
        "background-color: #2D3748; color: #D4AF37; font-family: 'Georgia', serif; "
        "font-weight: bold; font-size: 18px; border: 2px solid #D4AF37; "
        "border-radius: 6px; min-width: 65px; max-width: 65px; min-height: 65px; max-height: 65px;"
    );

    // Right Column: Identity Metadata
    QVBoxLayout *metaLayout = new QVBoxLayout();
    metaLayout->setSpacing(4);
    
    QLabel *titleLabel = new QLabel(QString("✨ %1").arg(QString::fromStdString(knight.getTitle())));
    titleLabel->setStyleSheet("font-size: 14px; font-style: italic; color: #CBD5E1; font-weight: bold;");
    
    QLabel *originLabel = new QLabel(QString("🚩 Allegiance: %1").arg(QString::fromStdString(knight.getOrigin())));
    originLabel->setStyleSheet("font-size: 13px; color: #A0AEC0;");

    metaLayout->addWidget(titleLabel);
    metaLayout->addWidget(originLabel);
    metaLayout->addStretch();

    topCardLayout->addWidget(crestBadge);
    topCardLayout->addLayout(metaLayout, 1);
    layout->addLayout(topCardLayout);

    // 2. BOTTOM VITALS GRID
    QGridLayout *vitalsGrid = new QGridLayout();
    vitalsGrid->setSpacing(10);

    auto createVitalsBox = [](const QString &metricTitle, const QString &metricValue) {
        QWidget *box = new QWidget();
        box->setStyleSheet("background-color: #2D3748; border: 1px solid #4A5568; border-radius: 4px;");
        QVBoxLayout *boxLayout = new QVBoxLayout(box);
        boxLayout->setContentsMargins(10, 8, 10, 8);
        boxLayout->setSpacing(2);

        QLabel *tLabel = new QLabel(metricTitle);
        tLabel->setStyleSheet("font-size: 11px; color: #A0AEC0; font-weight: bold; text-transform: uppercase;");
        QLabel *vLabel = new QLabel(metricValue);
        vLabel->setStyleSheet("font-size: 15px; color: white; font-weight: bold;");

        boxLayout->addWidget(tLabel);
        boxLayout->addWidget(vLabel);
        return box;
    };

    vitalsGrid->addWidget(createVitalsBox("Height", QString("%1 cm").arg(knight.getHeight())), 0, 0);
    vitalsGrid->addWidget(createVitalsBox("Weight", QString("%1 kg").arg(knight.getWeight())), 0, 1);
    
    layout->addLayout(vitalsGrid);
    layout->addStretch();
    return container;
}

QWidget* createEditableStatRow(Knight &knight, const QString &statName, Knight::Skill skill, int value, std::function<void()> onUpdateNeeded) {
    QWidget *container = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->setContentsMargins(5, 4, 5, 4);

    QLabel *label = new QLabel(statName, container);
    label->setStyleSheet("font-weight: bold; color: #CBD5E1; font-size: 13px;");
    
    QProgressBar *bar = new QProgressBar(container);
    bar->setRange(0, 100);
    bar->setValue(value);
    bar->setTextVisible(true);
    bar->setFormat("%v/100");
    bar->setStyleSheet(
        "QProgressBar { background-color: #2D3748; border: 1px solid #4A5568; border-radius: 4px; text-align: center; color: white; font-weight: bold; height: 24px; font-size: 11px; }"
        "QProgressBar::chunk { background-color: #D4AF37; border-radius: 2px; }"
    );

    layout->addWidget(label, 2);
    layout->addWidget(bar, 3);

    if (knight.getPendingSkillPoints() > 0 && value < 100) {
        QPushButton *plusBtn = new QPushButton("+", container);
        plusBtn->setFixedSize(24, 24);
        plusBtn->setStyleSheet(
            "QPushButton { background-color: #059669; color: white; font-weight: bold; font-size: 14px; border-radius: 4px; border: none; }"
            "QPushButton:hover { background-color: #10B981; }"
            "QPushButton:pressed { background-color: #047857; }"
        );
        
        QObject::connect(plusBtn, &QPushButton::clicked, [container, &knight, skill, onUpdateNeeded]() {
            if (knight.upgradeSkill(skill)) {
                onUpdateNeeded();
            }
        });
        layout->addWidget(plusBtn, 0);
    } else {
        layout->addSpacing(28); 
    }

    return container;
}

// Master Panel: Assembles and dynamically redraws the skill sheet when points change (FOR OWNED ROSTER KNIGHTS)
QWidget* createUnifiedCombatSkillsPanel(Knight &knight, QWidget *parentDialog) {
    QWidget *mainContainer = new QWidget();
    QVBoxLayout *masterLayout = new QVBoxLayout(mainContainer);
    masterLayout->setContentsMargins(0, 0, 0, 0);

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; background: transparent; }");
    
    QWidget *contentWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(contentWidget);
    layout->setContentsMargins(10, 5, 10, 5);
    layout->setSpacing(4);

    if (knight.getPendingSkillPoints() > 0) {
        QWidget *banner = new QWidget();
        banner->setStyleSheet("background-color: #065F46; border: 1px solid #10B981; border-radius: 6px;");
        QHBoxLayout *bannerLayout = new QHBoxLayout(banner);
        bannerLayout->setContentsMargins(12, 8, 12, 8);

        QLabel *bannerText = new QLabel(QString("✨ UNASSIGNED TRAINING POINTS:  %1").arg(knight.getPendingSkillPoints()));
        bannerText->setStyleSheet("color: #A7F3D0; font-weight: bold; font-size: 13px; font-family: 'Georgia', serif;");
        
        bannerLayout->addWidget(bannerText);
        layout->addWidget(banner);
    }

    auto refreshView = [parentDialog, &knight]() {
        QTabWidget *tabs = parentDialog->findChild<QTabWidget*>();
        if (tabs) {
            int currentIndex = tabs->currentIndex();
            QWidget *oldTab = tabs->widget(currentIndex);
            QWidget *newTab = createUnifiedCombatSkillsPanel(knight, parentDialog);
            
            tabs->removeTab(currentIndex);
            tabs->insertTab(currentIndex, newTab, "⚔️ Combat Training");
            tabs->setCurrentIndex(currentIndex);
            delete oldTab;
        }
    };

    layout->addWidget(createSectionHeader("Jousting Records", "🏇"));
    layout->addWidget(createEditableStatRow(knight, "Horsemanship", Knight::Skill::Horsemanship, knight.getHorsemanship(), refreshView));
    layout->addWidget(createEditableStatRow(knight, "Lance Precision", Knight::Skill::LancePrecision, knight.getLancePrecision(), refreshView));
    layout->addWidget(createEditableStatRow(knight, "Bracing Poise", Knight::Skill::Poise, knight.getPosie(), refreshView));

    layout->addWidget(createSectionHeader("Ground Melee Combat", "⚔️"));
    layout->addWidget(createEditableStatRow(knight, "Weapon Mastery", Knight::Skill::Swordplay, knight.getSwordplay(), refreshView));
    layout->addWidget(createEditableStatRow(knight, "Combat Footwork", Knight::Skill::Footwork, knight.getFootwork(), refreshView));
    layout->addWidget(createEditableStatRow(knight, "Physical Vigor", Knight::Skill::Vigor, knight.getVigor(), refreshView));

    layout->addWidget(createSectionHeader("Archery & Range Focus", "🎯"));
    layout->addWidget(createEditableStatRow(knight, "Marksmanship", Knight::Skill::Marksmanship, knight.getMarksmanship(), refreshView));
    layout->addWidget(createEditableStatRow(knight, "Mental Composure", Knight::Skill::Focus, knight.getFocus(), refreshView));

    layout->addStretch();
    
    scrollArea->setWidget(contentWidget);
    masterLayout->addWidget(scrollArea);
    return mainContainer;
}

// 🌟 NEW OVERLOAD: Read-Only Version (FOR RECRUITMENT PANELS)
QWidget* createUnifiedCombatSkillsPanel(const Knight &knight) {
    QWidget *mainContainer = new QWidget();
    QVBoxLayout *masterLayout = new QVBoxLayout(mainContainer);
    masterLayout->setContentsMargins(0, 0, 0, 0);

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; background: transparent; }");
    
    QWidget *contentWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(contentWidget);
    layout->setContentsMargins(10, 5, 10, 5);
    layout->setSpacing(4);

    layout->addWidget(createSectionHeader("Jousting Records", "🏇"));
    layout->addWidget(createStatRow("Horsemanship", knight.getHorsemanship()));
    layout->addWidget(createStatRow("Lance Precision", knight.getLancePrecision()));
    layout->addWidget(createStatRow("Bracing Poise", knight.getPosie()));

    layout->addWidget(createSectionHeader("Ground Melee Combat", "⚔️"));
    layout->addWidget(createStatRow("Weapon Mastery", knight.getSwordplay()));
    layout->addWidget(createStatRow("Combat Footwork", knight.getFootwork()));
    layout->addWidget(createStatRow("Physical Vigor", knight.getVigor()));

    layout->addWidget(createSectionHeader("Archery & Range Focus", "🎯"));
    layout->addWidget(createStatRow("Marksmanship", knight.getMarksmanship()));
    layout->addWidget(createStatRow("Mental Composure", knight.getFocus()));

    layout->addStretch();
    
    scrollArea->setWidget(contentWidget);
    masterLayout->addWidget(scrollArea);
    return mainContainer;
}

// ============================================================================
// 1. ROSTER VIEW CONSTRUCTOR (EDITABLE EQUIPMENT & STATS)
// ============================================================================
KnightDetailDialog::KnightDetailDialog(Player &player, Knight &knight, KnightRosterView *parent)
: GameDialog(parent), m_player(player)
{
    setMinimumSize(600, 550);
    setModal(true);

    QLabel *nameLabel = new QLabel(QString::fromStdString(knight.getName()), this);
    nameLabel->setStyleSheet("font-family: 'Georgia', serif; font-size: 26px; font-weight: bold; color: #D4AF37; margin-bottom: 5px;");
    mainLayout->addWidget(nameLabel);

    QTabWidget *tabs = new QTabWidget(this);

    tabs->addTab(createDynamicProfileCard(knight), "📋 Profile");

    QWidget *equipTab = new QWidget();
    QVBoxLayout *equipLayout = new QVBoxLayout(equipTab);
    equipLayout->setContentsMargins(10, 10, 10, 10);
    equipLayout->setSpacing(12);
    
    QLabel *eqHeader = new QLabel("Equipped Tourney Armaments:");
    eqHeader->setStyleSheet("font-weight: bold; color: #D4AF37; font-size: 14px;");
    equipLayout->addWidget(eqHeader);
    
    equipLayout->addWidget(createEquipmentSlot(knight, Item::ItemType::Armour));   
    equipLayout->addWidget(createEquipmentSlot(knight, Item::ItemType::Weapon));   
    
    QLabel *leftHandLabel = new QLabel("Left Hand: Empty (Shield Slot Requirement)", this);
    leftHandLabel->setStyleSheet("padding: 12px; background: #1A202C; border: 1px dashed #4A5568; border-radius: 4px; color: #718096; font-size: 13px;");
    equipLayout->addWidget(leftHandLabel);
    equipLayout->addStretch();
    tabs->addTab(equipTab, "🛡️ Equipment");

    // Calls mutable version with 'this' context link
    tabs->addTab(createUnifiedCombatSkillsPanel(knight, this), "⚔️ Combat Training");

    mainLayout->addWidget(tabs);

    QPushButton *closeButton = new QPushButton("Return to Roster", this);
    closeButton->setStyleSheet("padding: 12px; font-size: 14px; font-weight: bold; background-color: #2D3748; color: white; border: 1px solid #4A5568; border-radius: 4px; margin-top: 5px;");
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    mainLayout->addWidget(closeButton);
}

// ============================================================================
// 2. RECRUITMENT VIEW CONSTRUCTOR (READ-ONLY EQUIPMENT & STATS)
// ============================================================================
KnightDetailDialog::KnightDetailDialog(Player &player, const Knight &knight, KnightRecruitmentView *parent) 
: GameDialog(parent), m_player(player)
{
    setMinimumSize(600, 550);
    setModal(true);

    QLabel *nameLabel = new QLabel(QString::fromStdString(knight.getName()), this);
    nameLabel->setStyleSheet("font-family: 'Georgia', serif; font-size: 26px; font-weight: bold; color: #D4AF37; margin-bottom: 5px;");
    mainLayout->addWidget(nameLabel);

    QTabWidget *tabs = new QTabWidget(this);

    tabs->addTab(createDynamicProfileCard(knight), "📋 Profile");

    QWidget *equipTab = new QWidget();
    QVBoxLayout *equipLayout = new QVBoxLayout(equipTab);
    equipLayout->setContentsMargins(10, 10, 10, 10);
    equipLayout->setSpacing(8);
    
    QLabel *infoNotice = new QLabel("⚠️ Hire this mercenary to manage armaments:");
    infoNotice->setStyleSheet("color: #FBBF24; font-weight: bold; font-size: 13px; margin-bottom: 5px;");
    equipLayout->addWidget(infoNotice);
    
    QString bodyGear = QString("👕 Torso Plate: %1").arg(QString::fromStdString(knight.getArmour().getName()));
    QString weaponGear = QString("⚔️ Main Weapon: %1").arg(QString::fromStdString(knight.getRightHandWeapon().getName()));

    QLabel *bodyLabel = new QLabel(bodyGear, this);
    bodyLabel->setStyleSheet("padding: 12px; background: #2D3748; border-radius: 4px; border-left: 4px solid #D4AF37; font-size: 13px; color: white;");
    QLabel *weaponLabel = new QLabel(weaponGear, this);
    weaponLabel->setStyleSheet("padding: 12px; background: #2D3748; border-radius: 4px; border-left: 4px solid #D4AF37; font-size: 13px; color: white;");
    QLabel *leftLabel = new QLabel("🛡️ Offhand Slot: Unassigned", this);
    leftLabel->setStyleSheet("padding: 12px; background: #2D3748; border-radius: 4px; border-left: 4px solid #4A5568; font-size: 13px; color: #A0AEC0;");

    equipLayout->addWidget(bodyLabel);
    equipLayout->addWidget(weaponLabel);
    equipLayout->addWidget(leftLabel);
    equipLayout->addStretch();
    tabs->addTab(equipTab, "🛡️ armaments");

    tabs->addTab(createUnifiedCombatSkillsPanel(knight), "⚔️ Combat Training");

    mainLayout->addWidget(tabs);

    QHBoxLayout *actionsLayout = new QHBoxLayout();
    actionsLayout->setSpacing(10);
    
    QPushButton *buyButton = new QPushButton(QString("📜 Sign Contract for: %1").arg(knight.getCost()), this);
    buyButton->setStyleSheet("padding: 12px; font-size: 14px; font-weight: bold; background-color: #059669; color: white; border-radius: 4px;");
    connect(buyButton, &QPushButton::clicked, this, [this]() { this->accept(); });
    
    QPushButton *closeButton = new QPushButton("Dismiss", this);
    closeButton->setStyleSheet("padding: 12px; font-size: 14px; font-weight: bold; background-color: #4B5563; color: white; border-radius: 4px;");
    connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);
    
    actionsLayout->addWidget(buyButton, 2);
    actionsLayout->addWidget(closeButton, 1);
    mainLayout->addLayout(actionsLayout);
}