#ifndef _Manager_Interface
#define _Manager_Interface

#pragma once

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "Knights/KnightRosterTab.h"
#include "Knights/KnightRecruitmentTab.h"
#include "Tournaments/TournamentTab.h"
#include "GameTimelineController.h"

class ManagerInterface : public QWidget {
    Q_OBJECT

public:
    explicit ManagerInterface(QWidget *parent = nullptr);

    void refreshDashboardUI();

private:
    QTabWidget *tabWidget;
    GameTimelineController *gameController;

    // Helper functions to keep our panel generation modular
    KnightRosterTab* knightRosterTab;
    KnightRecruitmentTab* knightRecruitmentTab;
    QWidget* createShopTab();
    TournamentTab* tournamentTab;
};
#endif