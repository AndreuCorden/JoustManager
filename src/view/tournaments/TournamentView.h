#ifndef TOURNAMENTVIEW_H
#define TOURNAMENTVIEW_H

#pragma once

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "model/knights/Knight.h"
#include "model/tournaments/Tournament.h"
#include "Player.h"

#include <vector>

class TournamentView : public QWidget
{
    Q_OBJECT
public:
    explicit TournamentView(Player &player, QWidget *parent = nullptr);

    void populateRoster(std::vector<Tournament> const &todaysTournaments);

signals:
    void registrationRequested(size_t index, std::vector<Knight> const & registeredKnights);

    void cancellationRequested(size_t index);

private:
    QVBoxLayout *m_listLayout;

    Player& m_player;
};

#endif