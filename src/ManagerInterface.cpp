#include "ManagerInterface.h"
#include "Knights/KnightRosterTab.h"

ManagerInterface::ManagerInterface(QWidget *parent) : QWidget(parent) {
    // Main structural layout for this view
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Create the Tab Container
    tabWidget = new QTabWidget(this);
    tabWidget->setStyleSheet("QTabBar::tab { font-size: 16px; padding: 10px 20px; }");

    // Generate our sub-panels and plug them into the tabs
    knightsTab = new KnightRosterTab();
    tabWidget->addTab(knightsTab, "Knights Roster");
    tabWidget->addTab(createRecruitmentTab(),"Knight Recruitment");
    tabWidget->addTab(createShopTab(), "Blacksmith Shop");
    tabWidget->addTab(createTournamentTab(), "Tournament Arena");

    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
}

QWidget* ManagerInterface::createRecruitmentTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);

    QLabel *label = new QLabel("AVAILABLE KNIGHTS");
    label->setStyleSheet("font-size: 20px; font-weight: bold; color: #4A5568;");
    
    QPushButton *knightButton = new QPushButton("Sir Robert (View Stats & Gear)", tab);
    knightButton->setStyleSheet("padding: 12px; font-size: 14px;");

    layout->addWidget(label);
    layout->addWidget(knightButton);
    layout->addStretch();

    tab->setLayout(layout);
    return tab;
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

QWidget* ManagerInterface::createTournamentTab() {
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);

    QLabel *label = new QLabel("UPCOMING TOURNAMENTS", tab);
    label->setStyleSheet("font-size: 20px; font-weight: bold; color: #4A5568;");
    
    QPushButton *enterTourney = new QPushButton("Enter the King's Grand Tournament", tab);
    enterTourney->setStyleSheet("padding: 15px; background-color: #D4AF37; color: black; font-weight: bold;");

    layout->addWidget(label);
    layout->addWidget(enterTourney);
    layout->addStretch();

    tab->setLayout(layout);
    return tab;
}