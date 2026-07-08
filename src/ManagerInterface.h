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
#include "controller/tournaments/TournamentTabController.h"
#include "GameTimelineController.h"
#include "Purse.h"

class ManagerInterface : public QWidget
{
    Q_OBJECT

public:
    explicit ManagerInterface(QWidget *parent);

    void runJousts();

private slots:
    void startNextDay();
    void claimReward(unsigned reward);
    void processPayment(int amount, bool &approved);

private:
    void updateHeaderDisplays();

    GameTimelineController *m_gameTimelineController;

    QLabel *m_dayLabel;
    QLabel *m_goldLabel;

    QTabWidget *m_tabWidget;

    // Helper functions to keep our panel generation modular
    KnightRosterTabController *m_knightRosterTabController;
    KnightRecruitmentTabController *m_knightRecruitmentTabController;
    ShopTabController *m_shopTabController;
    TournamentTabController *m_tournamentTabController;

    Purse m_purse;
};

#endif