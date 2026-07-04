#include <QRandomGenerator>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include <vector>
#include <string>
#include "GameTimelineController.h"
#include "Tournaments/Joust/JoustView.h"
#include "Player.h"
#include "Items/Armour/GothicArmour.h"
#include "Items/Weapon/CourteousLance.h"
#include "GameDialog.h"

GameTimelineController::GameTimelineController(QObject *parent)
    : QObject(parent), currentDay(1)
{
    // Roll items for Day 1
    generateDailyPools();
}

void GameTimelineController::generateDailyPools()
{
    availableRecruits.clear();
    availableShopItems.clear();
    availableTournaments.clear();

    auto *rand = QRandomGenerator::global();

    // 1. Generate 3 Random Knights to Hire
    std::vector<std::string> knightNames = {"Gawain", "Lancelot", "Bors", "Galahad", "Percival"};
    std::vector<std::string> knightLocations = {"English", "French", "Bavaria", "Spain", "Scotland"};
    std::vector<std::string> knightTitles = {"Knight", "Chivalric Knight", "Baron", "Count", "Earl", "Duke", "Prince"};
    for (int i = 0; i < 3; ++i)
    {
        Knight k(knightNames[rand->bounded(knightNames.size())],
                 knightLocations[rand->bounded(knightLocations.size())],
                 knightTitles[rand->bounded(knightTitles.size())],
                 rand->bounded(150, 210),
                 rand->bounded(70, 90),
                 rand->bounded(100, 500));
        // Set basic random stats scaled slightly by day progression
        availableRecruits.push_back(k);
    }

    // 2. Generate 3 Random Shop Items
    availableShopItems.push_back(GothicArmour());
    availableShopItems.push_back(CourteousLance());

    // 3. Generate 2 Random Tournament Tiers
    Tournament localTourney = {"The King's Grand Tournament", 2, 3, 500, "King Arthur"};
    Tournament grandTourney = {"Squire's Melee Brawl", 1, 2, 120, "Lord Cenric"};

    // (Populate enemy squads based on difficulty here...)
    availableTournaments.push_back(localTourney);
    availableTournaments.push_back(grandTourney);
}

void GameTimelineController::registerForTournament(int tournamentIndex)
{
    if (tournamentIndex >= 0 && tournamentIndex < (int)availableTournaments.size())
    {
        // Push selected tournament straight into tomorrow's processing queue
        tournamentQueue.push(availableTournaments[tournamentIndex]);
    }
}

void GameTimelineController::triggerNextDay()
{
    // Kick off sequential animations of all signed-up matches
    runNextRegisteredTournament();
}

void GameTimelineController::runNextRegisteredTournament()
{
    // Base Case: If no tournaments are left in today's queue, advance the timeline safely
    if (tournamentQueue.empty())
    {
        currentDay++;
        generateDailyPools();
        emit dayAdvanced(); // UI intercepts this to refresh text assets and storefronts
        return;
    }

    // Grab the next tournament in line
    Tournament activeMatch = tournamentQueue.front();
    tournamentQueue.pop();

    activeMatch.generateCurrentRoundMatches();

    // 🎥 Spin up your JoustView inside a modal dialog window container
    GameDialog *matchDialog = new GameDialog();
    matchDialog->setStyleSheet("background-color: #2D3748;");

    QVBoxLayout *layout = new QVBoxLayout(matchDialog);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    QLabel *titleLabel = new QLabel(QString::fromStdString(activeMatch.getName()), matchDialog);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 26px; font-weight: bold; color: #F6AD55; margin-bottom: 10px;");

    QLabel *hostLabel = new QLabel(QString::fromStdString(activeMatch.getHost()), matchDialog);
    hostLabel->setAlignment(Qt::AlignCenter);
    hostLabel->setStyleSheet("font-size: 16px; color: #E2E8F0; font-style: italic;");

    QPushButton *advanceButton = new QPushButton("Advance to Round", matchDialog);
    advanceButton->setCursor(Qt::PointingHandCursor);
    advanceButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #48BB78; color: white; font-size: 16px; font-weight: bold;"
        "   padding: 12px 30px; border-radius: 6px; border: none;"
        "}"
        "QPushButton:hover { background-color: #38A169; }");

    layout->addWidget(titleLabel);
    layout->addWidget(hostLabel);
    layout->addWidget(advanceButton);

    QObject::connect(advanceButton, &QPushButton::clicked, [this, activeMatch, matchDialog, layout, titleLabel, hostLabel, advanceButton]() mutable
                     {
        
        // Step A: Trigger match creation logic right now on click
        activeMatch.generateCurrentRoundMatches();

        // Step B: Clean up and remove the introductory UI assets
        layout->removeWidget(titleLabel);
        layout->removeWidget(hostLabel);
        layout->removeWidget(advanceButton);
        
        titleLabel->deleteLater();
        hostLabel->deleteLater();
        advanceButton->deleteLater(); });

    // 4. Clean up structural framework when matches terminate completely
    QObject::connect(matchDialog, &QDialog::accepted, [this, activeMatch, matchDialog]()
                     {
        Player::getInstance().modifyGold(activeMatch.getReward()); // Collect gold
        matchDialog->deleteLater();

        // RECURSIVE CALL: Keep executing the loop until the queue clears completely
        this->runNextRegisteredTournament(); });

    matchDialog->exec();
}