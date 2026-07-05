#include "ManagerInterface.h"
#include "Knights/KnightRosterTab.h"
#include "Knights/KnightRecruitmentTab.h"
#include "Tournaments/TournamentTab.h"
#include "GameTimelineController.h"

ManagerInterface::ManagerInterface(QWidget *parent, GameTimelineController &gameTimelineController) 
: QWidget(parent)
, m_gameTimelineController(gameTimelineController)
, m_tabWidget(new QTabWidget(this))
, m_knightRosterTab(new KnightRosterTab(this))
, m_knightRecruitmentTab(new KnightRecruitmentTab(this))
, m_tournamentTab(new TournamentTab(this))
{
    // Main structural layout for this view
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    /*dayLabel = new QLabel(QString("CURRENT DAY: %1").arg(gameController.getCurrentDay()), this);
    dayLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #D4AF37; padding: 5px;");
    mainLayout->addWidget(dayLabel);*/

    // Create the Tab Container
    m_tabWidget->setStyleSheet("QTabBar::tab { font-size: 16px; padding: 10px 20px; }");


    m_tabWidget->addTab(m_knightRosterTab, "Knights Roster");
    m_tabWidget->addTab(m_knightRecruitmentTab, "Knight Recruitment");
    m_tabWidget->addTab(createShopTab(), "Blacksmith Shop");
    m_tabWidget->addTab(m_tournamentTab, "Tournament Arena");

    mainLayout->addWidget(m_tabWidget);
    setLayout(mainLayout);

    connect(m_tabWidget, &QTabWidget::currentChanged, this, [this](int index)
            {
        // Since "Knights Roster" was added first, its index is 0
        if (index == 0) {
            m_knightRosterTab->populateRoster();
        } });

    QPushButton *nextDayButton = new QPushButton("Next Day", this);
    nextDayButton->setStyleSheet("padding: 10px 20px; background-color: #D4AF37; color: black; font-weight: bold;");
    mainLayout->addWidget(nextDayButton);

    connect(nextDayButton, &QPushButton::clicked, 
        &this->m_gameTimelineController, &GameTimelineController::triggerNextDay);

    connect(&GameTimelineController::getInstance(), &GameTimelineController::dayAdvanced, 
            this, &ManagerInterface::refreshDashboardUI);
}

QWidget *ManagerInterface::createShopTab()
{
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

void ManagerInterface::refreshDashboardUI()
{
    // 🌟 Update visual day text using the singleton instance
    // dayLabel->setText(QString("CURRENT DAY: %1").arg(GameTimelineController::getInstance().getCurrentDay()));

    m_knightRosterTab->populateRoster();
    m_knightRecruitmentTab->startDay();
    m_tournamentTab->populateRoster(); 
}