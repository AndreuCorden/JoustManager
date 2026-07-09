#include "ManagerInterface.h"

ManagerInterface::ManagerInterface(QWidget *parent)
    : QWidget(parent)
    , m_player(Player())
    , m_purse(1000)
    , m_gameTimelineController(new GameTimelineController())
    , m_tabWidget(new QTabWidget(this))
    , m_knightRosterTabController(new KnightRosterTabController(m_player, this))
    , m_knightRecruitmentTabController(new KnightRecruitmentTabController(m_player, this))
    , m_shopTabController(new ShopTabController(m_player, this))
    , m_tournamentTabController(new TournamentTabController(this, m_player, m_gameTimelineController->getTodaysTournaments()))
{
    // Main structural layout for this view
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setContentsMargins(10, 5, 10, 15);

    m_dayLabel = new QLabel(this);
    m_dayLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #E2E8F0;");

    m_goldLabel = new QLabel(this);
    m_goldLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #D4AF37;");

    headerLayout->addWidget(m_dayLabel);
    headerLayout->addStretch(); // Pushes the gold display completely to the right edge
    headerLayout->addWidget(m_goldLabel);

    mainLayout->addLayout(headerLayout);

    // Create the Tab Container
    m_tabWidget->setStyleSheet("QTabBar::tab { font-size: 16px; padding: 10px 20px; }");

    m_tabWidget->addTab(m_knightRosterTabController->getTab(), "Knights Roster");
    m_tabWidget->addTab(m_knightRecruitmentTabController->getTab(), "Knight Recruitment");
    m_tabWidget->addTab(m_shopTabController->getTab(), "Blacksmith Shop");
    m_tabWidget->addTab(m_tournamentTabController->getTab(), "Tournament Arena");

    mainLayout->addWidget(m_tabWidget);
    setLayout(mainLayout);

    connect(m_knightRecruitmentTabController, &KnightRecruitmentTabController::requestPayment,
            this, &ManagerInterface::processPayment);

    connect(m_shopTabController, &ShopTabController::requestPayment,
            this, &ManagerInterface::processPayment);

    connect(m_tournamentTabController, &TournamentTabController::giveReward,
            this, &ManagerInterface::claimReward);

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
            this, &ManagerInterface::runJousts);

    connect(m_tournamentTabController, &TournamentTabController::endOfJousting,
            this, &ManagerInterface::startNextDay);

    updateHeaderDisplays();
}

void ManagerInterface::runJousts()
{
    m_tournamentTabController->runTournaments();
}

void ManagerInterface::startNextDay()
{
    m_knightRosterTabController->startDay();
    m_knightRecruitmentTabController->startDay();
    m_shopTabController->startDay();
    m_tournamentTabController->setDailyTournaments(m_gameTimelineController->getTodaysTournaments());

    updateHeaderDisplays();
}

void ManagerInterface::processPayment(int amount, bool &approved)
{
    approved = m_purse.buy(amount);
    if (approved) {
        updateHeaderDisplays(); 
    }
}

void ManagerInterface::claimReward(unsigned reward)
{
    m_purse += reward;
    updateHeaderDisplays();
}

void ManagerInterface::updateHeaderDisplays()
{
    // Grab the data values straight from your tracking model configurations
    int currentDay = m_gameTimelineController->getCurrentDay();
    unsigned int currentGold = m_purse.get();

    m_dayLabel->setText(QString("📅  DAY: %1").arg(currentDay));
    m_goldLabel->setText(QString("🪙  GOLD: %1g").arg(currentGold));
}