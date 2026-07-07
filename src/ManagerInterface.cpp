#include "ManagerInterface.h"

ManagerInterface::ManagerInterface(QWidget *parent) 
: QWidget(parent)
, m_gameTimelineController(new GameTimelineController())
, m_tabWidget(new QTabWidget(this))
, m_knightRosterTabController(new KnightRosterTabController(this))
, m_knightRecruitmentTabController(new KnightRecruitmentTabController(this))
, m_shopTabController(new ShopTabController(this))
, m_tournamentTab(new TournamentTab((*m_gameTimelineController),this))
{
    // Main structural layout for this view
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    /*dayLabel = new QLabel(QString("CURRENT DAY: %1").arg(gameController.getCurrentDay()), this);
    dayLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #D4AF37; padding: 5px;");
    mainLayout->addWidget(dayLabel);*/

    // Create the Tab Container
    m_tabWidget->setStyleSheet("QTabBar::tab { font-size: 16px; padding: 10px 20px; }");


    m_tabWidget->addTab(m_knightRosterTabController->getTab(), "Knights Roster");
    m_tabWidget->addTab(m_knightRecruitmentTabController->getTab(), "Knight Recruitment");
    m_tabWidget->addTab(m_shopTabController->getTab(), "Blacksmith Shop");
    m_tabWidget->addTab(m_tournamentTab, "Tournament Arena");

    mainLayout->addWidget(m_tabWidget);
    setLayout(mainLayout);

    connect(m_tabWidget, &QTabWidget::currentChanged, this, [this](int index)
            {
        // Since "Knights Roster" was added first, its index is 0
        if (index == 0) {
            m_knightRosterTabController->startDay();
        } });

    QPushButton *nextDayButton = new QPushButton("Next Day", this);
    nextDayButton->setStyleSheet("padding: 10px 20px; background-color: #D4AF37; color: black; font-weight: bold;");
    mainLayout->addWidget(nextDayButton);

    connect(nextDayButton, &QPushButton::clicked, 
        m_gameTimelineController, &GameTimelineController::triggerNextDay);

    connect(m_gameTimelineController, &GameTimelineController::dayAdvanced, 
            this, &ManagerInterface::refreshDashboardUI);
}

void ManagerInterface::refreshDashboardUI()
{
    // 🌟 Update visual day text using the singleton instance
    // dayLabel->setText(QString("CURRENT DAY: %1").arg(m_gameTimelineController->getCurrentDay()));

    m_knightRosterTabController->startDay();
    m_knightRecruitmentTabController->startDay();
    m_shopTabController->startDay();
    m_tournamentTab->populateRoster(); 
}