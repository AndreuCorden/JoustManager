#ifndef MANAGERINTERFACE_H
#define MANAGERINTERFACE_H

#pragma once

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "controller/knights/KnightRosterTabController.h"
#include "controller/knights/KnightRecruitmentTabController.h"
#include "controller/items/ShopTabController.h"
#include "Tournaments/TournamentTab.h"
#include "GameTimelineController.h"

class ManagerInterface : public QWidget {
    Q_OBJECT

public:
    explicit ManagerInterface(QWidget *parent);

    void refreshDashboardUI();

private:
    GameTimelineController* m_gameTimelineController;

    QTabWidget *m_tabWidget;

    // Helper functions to keep our panel generation modular
    KnightRosterTabController* m_knightRosterTabController;
    KnightRecruitmentTabController* m_knightRecruitmentTabController;
    ShopTabController* m_shopTabController;
    TournamentTab* m_tournamentTab;
};
#endif