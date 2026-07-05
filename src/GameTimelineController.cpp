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
#include "Tournaments/TournamentRunnerDialog.h"

#include <algorithm>

GameTimelineController::GameTimelineController(QObject *parent)
    : QObject(nullptr), currentDay(1)
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
        tournamentVector.push_back(availableTournaments[tournamentIndex]);
    }
}

void GameTimelineController::cancelTournamentRegistration(const std::string& tournamentName)
{
    tournamentVector.erase(
        std::remove_if(tournamentVector.begin(), tournamentVector.end(),
            [&tournamentName](const Tournament& t) {
                return t.getName() == tournamentName;
            }),
        tournamentVector.end()
    );
}

void GameTimelineController::triggerNextDay()
{
    // Kick off sequential animations of all signed-up matches
    runNextRegisteredTournament(0);
}

// 🌟 The Correct, Stable Pattern (Sequential State Machine)
void GameTimelineController::runNextRegisteredTournament(size_t currentIndex)
{
    // 1. BASE CASE: If we processed every item in the vector queue, reset and advance!
    if (currentIndex >= tournamentVector.size())
    {
        tournamentVector.clear(); // 🧼 Clean everything out safely at the very end
        currentDay++;
        generateDailyPools();
        emit dayAdvanced(); 
        return;
    }

    // 2. Grab a perfectly safe, stable reference using the tracking index
    Tournament& activeMatch = tournamentVector[currentIndex];

    TournamentRunnerDialog *runner = new TournamentRunnerDialog(activeMatch, nullptr);
    
    QObject::connect(runner, &QDialog::accepted, [this, currentIndex, runner]() {
        // Collect rewards from the stable vector slot
        int rewardAmount = tournamentVector[currentIndex].getReward();
        Player::getInstance().modifyGold(rewardAmount); 
        
        runner->deleteLater();

        // 3. RECURSION STEP: Safely pass the NEXT index integer forward 
        this->runNextRegisteredTournament(currentIndex + 1); 
    });

    runner->exec(); 
}