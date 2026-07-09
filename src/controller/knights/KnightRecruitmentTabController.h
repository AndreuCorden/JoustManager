#ifndef KNIGHTRECRUITMENTTABCONTROLLER_H
#define KNIGHTRECRUITMENTTABCONTROLLER_H

#pragma once

#include <QObject>
#include <QWidget>

#include "model/knights/KnightRecruitmentTabHandler.h"
#include "view/knights/KnightRecruitmentView.h"
#include "Player.h"

class KnightRecruitmentTabController : public QObject
{
    Q_OBJECT
public:
    KnightRecruitmentTabController(Player &player, QWidget *parent = nullptr);

    KnightRecruitmentView* getTab() { return m_KnightRecruitmentView; }

    void startDay();

signals:
    void requestPayment(int amount, bool &approved);

private slots:
    void handleKnightRecruitment(const Knight &knight);

private:
    KnightRecruitmentView *m_KnightRecruitmentView;
    KnightRecruitmentTabHandler* m_KnightRecruitmentTabHandler;

    Player &m_player;
};

#endif