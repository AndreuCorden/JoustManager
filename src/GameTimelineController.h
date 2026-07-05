#ifndef GAMETIMELINECONTROLLER_H
#define GAMETIMELINECONTROLLER_H

#pragma once
#include <vector>
#include <queue>
#include <QString>
#include <QObject>
#include "Knights/Knight.h"
#include "Tournaments/Tournament.h"
#include "Items/Item.h"

class GameTimelineController : public QObject
{
    Q_OBJECT

public:
    static GameTimelineController &getInstance()
    {
        static GameTimelineController instance;
        return instance;
    }
    GameTimelineController(const GameTimelineController &) = delete;
    void operator=(const GameTimelineController &) = delete;

    // --- Daily Actions Engine ---
    void hireKnight(int poolIndex);
    void buyEquipment(int shopIndex, int knightIndex);
    void registerForTournament(int tournamentIndex);
    void cancelTournamentRegistration(const std::string& tournamentIndex);
    void triggerNextDay();

    // --- Getters for UI Panels ---
    int getCurrentDay() const { return currentDay; }
    const std::vector<Knight> &getAvailableRecruits() const { return availableRecruits; }
    const std::vector<Item> &getAvailableShopItems() const { return availableShopItems; }
    std::vector<Tournament> &getAvailableTournaments() { return availableTournaments; }

signals:
    void dayAdvanced(); // Notify UI to refresh all shops, lists, and gold displays

private:
    explicit GameTimelineController(QObject *parent = nullptr);
    ~GameTimelineController() override = default;

    void generateDailyPools();
    void runNextRegisteredTournament(size_t currentIndex);

    // Player State
    int currentDay;

    // Daily Regenerating Pools
    std::vector<Knight> availableRecruits;
    std::vector<Item> availableShopItems;
    std::vector<Tournament> availableTournaments;

    // Active Day Queue Processing
    std::vector<Tournament> tournamentVector;
};

#endif // GAMETIMELINECONTROLLER_H