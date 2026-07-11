#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include <vector>
#include <string>
#include "GameTimelineController.h"
#include "Player.h"
#include "model/items/Armour/GothicArmour.h"
#include "model/items/Weapon/CourteousLance.h"
#include "GameDialog.h"

#include <algorithm>

GameTimelineController::GameTimelineController(QObject *parent)
    : QObject(nullptr), m_currentDay(1)
{
    // Roll items for Day 1
    generateDailyPools();
}

void GameTimelineController::generateDailyPools()
{
    // 3. Generate 2 Random Tournament Tiers
    Tournament localTourney = {"The King's Grand Tournament", 2, 3, 500, "King Arthur", 50};
    Tournament grandTourney = {"Squire's Melee Brawl", 1, 2, 120, "Lord Cenric", 1000};

    std::vector<Tournament> todaysTournaments;

    // (Populate enemy squads based on difficulty here...)
    todaysTournaments.push_back(localTourney);
    todaysTournaments.push_back(grandTourney);

    m_upcommingTournaments.push(todaysTournaments);
}

void GameTimelineController::triggerNextDay()
{
    // Kick off sequential animations of all signed-up matches
    ++m_currentDay;
    // Temporarilly called here to have a constant flow of tournaments
    generateDailyPools();
    emit dayAdvanced();
}

std::vector<Tournament> GameTimelineController::getTodaysTournaments()
{  
    auto todaysTournament = std::move(m_upcommingTournaments.front());
    m_upcommingTournaments.pop();
    return todaysTournament;
}