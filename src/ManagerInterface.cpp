#include "ManagerInterface.h"
#include "Knights/KnightRosterTab.h"
#include "Knights/KnightRecruitmentTab.h"
#include "Tournaments/TournamentTab.h"

ManagerInterface::ManagerInterface(QWidget *parent) : QWidget(parent) {
    // Main structural layout for this view
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Create the Tab Container
    tabWidget = new QTabWidget(this);
    tabWidget->setStyleSheet("QTabBar::tab { font-size: 16px; padding: 10px 20px; }");

    // Generate our sub-panels and plug them into the tabs
    knightRosterTab = new KnightRosterTab();
    knightRecruitmentTab = new KnightRecruitmentTab();

    tournamentTab = new TournamentTab();
    
    tabWidget->addTab(knightRosterTab, "Knights Roster");
    tabWidget->addTab(knightRecruitmentTab,"Knight Recruitment");
    tabWidget->addTab(createShopTab(), "Blacksmith Shop");
    tabWidget->addTab(tournamentTab, "Tournament Arena");

    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
    
    connect(tabWidget, &QTabWidget::currentChanged, this, [this](int index) {
        // Since "Knights Roster" was added first, its index is 0
        if (index == 0) {
            knightRosterTab->populateRoster();
        }
    });
}

QWidget* ManagerInterface::createShopTab() {
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);

    QLabel *label = new QLabel("ARMOURY & BLACKSMITH", tab);
    label->setStyleSheet("font-size: 20px; font-weight: bold; color: #4A5568;");
    
    QPushButton *buyWeapon = new QPushButton("Buy Steel Lance - 50 Gold", tab);
    QPushButton *buyArmor = new QPushButton("Buy Plate Armour - 120 Gold", tab);
    buyWeapon->setStyleSheet("padding: 10px; margin: 5px;");
    buyArmor->setStyleSheet("padding: 10px; margin: 5px;");

    layout->addWidget(label);
    layout->addWidget(buyWeapon);
    layout->addWidget(buyArmor);
    layout->addStretch();

    tab->setLayout(layout);
    return tab;
}