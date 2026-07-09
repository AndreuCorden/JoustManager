#ifndef KNIGHTRECRUITMENTVIEW_H
#define KNIGHTRECRUITMENTVIEW_H

#pragma once

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "model/knights/Knight.h"
#include "Player.h"

#include <list>

class KnightRecruitmentView : public QWidget
{
    Q_OBJECT
public:
    explicit KnightRecruitmentView(Player& player, QWidget *parent = nullptr);

    void populateList(const std::list<Knight> &recruitableKnights);

signals:
    void knightRecruited(const Knight &knight);

private:
    QVBoxLayout *listLayout;

    Player& m_player;
};

#endif