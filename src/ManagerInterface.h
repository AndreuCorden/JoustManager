#ifndef MANAGERINTERFACE_H
#define MANAGERINTERFACE_H

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
    explicit ManagerInterface(QWidget *parent, GameTimelineController &gameTimelineController);

    void refreshDashboardUI();

private:
    GameTimelineController &m_gameTimelineController;

    QTabWidget *m_tabWidget;

    // Helper functions to keep our panel generation modular
    KnightRosterTab* m_knightRosterTab;
    KnightRecruitmentTab* m_knightRecruitmentTab;
    QWidget* createShopTab();
    TournamentTab* m_tournamentTab;
};
#endif