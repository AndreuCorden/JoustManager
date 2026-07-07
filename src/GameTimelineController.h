#ifndef GAMETIMELINECONTROLLER_H
#define GAMETIMELINECONTROLLER_H

#pragma once
#include <vector>
#include <queue>
#include <QString>
#include <QObject>
#include "Knights/Knight.h"
#include "Tournaments/Tournament.h"
#include "model/items/Item.h"

class GameTimelineController : public QObject
{
    Q_OBJECT

public:
    explicit GameTimelineController(QObject *parent = nullptr);
    ~GameTimelineController() override = default;
    GameTimelineController(const GameTimelineController &) = delete;
    void operator=(const GameTimelineController &) = delete;

    // --- Daily Actions Engine ---
    void registerForTournament(int tournamentIndex);
    void cancelTournamentRegistration(const std::string& tournamentIndex);
    void triggerNextDay();

    // --- Getters for UI Panels ---
    int getCurrentDay() const { return currentDay; }
    const std::vector<Item> &getAvailableShopItems() const { return availableShopItems; }
    std::vector<Tournament> &getAvailableTournaments() { return availableTournaments; }

signals:
    void dayAdvanced(); // Notify UI to refresh all shops, lists, and gold displays

private:

    void generateDailyPools();
    void runNextRegisteredTournament(size_t currentIndex);

    // Player State
    int currentDay;

    // Daily Regenerating Pools
    std::vector<Item> availableShopItems;
    std::vector<Tournament> availableTournaments;

    // Active Day Queue Processing
    std::vector<Tournament> tournamentVector;
};

#endif // GAMETIMELINECONTROLLER_H