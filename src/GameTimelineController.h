#ifndef GAMETIMELINECONTROLLER_H
#define GAMETIMELINECONTROLLER_H

#pragma once
#include <vector>
#include <queue>
#include <QString>
#include <QObject>
#include "model/knights/Knight.h"
#include "model/items/Item.h"
#include "model/tournaments/Tournament.h"

class GameTimelineController : public QObject
{
    Q_OBJECT

public:
    explicit GameTimelineController(QObject *parent = nullptr);
    ~GameTimelineController() override = default;
    GameTimelineController(const GameTimelineController &) = delete;
    void operator=(const GameTimelineController &) = delete;

    // --- Daily Actions Engine ---
    void triggerNextDay();

    // --- Getters for UI Panels ---
    int getCurrentDay() const { return m_currentDay; }
    std::vector<Tournament> getTodaysTournaments();

signals:
    void dayAdvanced(); // Notify UI to refresh all shops, lists, and gold displays

private:

    void generateDailyPools();
    void runNextRegisteredTournament(size_t currentIndex);

    // Player State
    int m_currentDay;

    std::queue<std::vector<Tournament>> m_upcommingTournaments;
};

#endif // GAMETIMELINECONTROLLER_H