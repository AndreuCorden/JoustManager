#ifndef TOURNAMENTRUNNERDIALOG_H
#define TOURNAMENTRUNNERDIALOG_H

#pragma once
#include "GameDialog.h"
#include "model/tournaments/Tournament.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScreen>
#include <QGuiApplication>

class TournamentRunnerDialog : public GameDialog
{
    Q_OBJECT

public:
    explicit TournamentRunnerDialog(Tournament &tournament, QWidget *parent = nullptr);
    ~TournamentRunnerDialog() override = default;

    void runTournamentRound();

signals:
    void nextRoundRequested();

private:
    void setupIntroScreen();
    void setupRewardsScreen();
    void clearCurrentLayout();

    Tournament &activeTournament;

    // Persistent pointers to easily clear states
    QLabel *titleLabel{nullptr};
    QLabel *infoLabel{nullptr};
    QPushButton *actionButton{nullptr};
};

#endif // TOURNAMENTRUNNERDIALOG_H