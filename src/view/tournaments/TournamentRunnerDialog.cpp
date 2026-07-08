#include "TournamentRunnerDialog.h"
#include "Player.h"

#include <iostream>

TournamentRunnerDialog::TournamentRunnerDialog(Tournament &tournament, QWidget *parent)
    : GameDialog(parent)
    , activeTournament(tournament)
{
    this->setMinimumSize(800, 600);
    this->resize(800, 600);

    if (this->mainLayout)
    {
        this->mainLayout->setAlignment(Qt::AlignCenter);
        this->mainLayout->setSpacing(20);
    }

    setupIntroScreen();
}

void TournamentRunnerDialog::setupIntroScreen()
{
    clearCurrentLayout();

    if (!mainLayout)
        return;

    titleLabel = new QLabel(QString("%1").arg(QString::fromStdString(activeTournament.getName())), this);
    titleLabel->setStyleSheet("font-size: 26px; font-weight: bold; color: #F6AD55; background: transparent;");
    titleLabel->setAlignment(Qt::AlignCenter);

    infoLabel = new QLabel(QString("Hosted by: %1")
                               .arg(QString::fromStdString(activeTournament.getHost())),
                           this);
    infoLabel->setStyleSheet("font-size: 16px; color: #E2E8F0; font-style: italic; background: transparent;");
    infoLabel->setAlignment(Qt::AlignCenter);

    actionButton = new QPushButton("Begin Tournament", this);
    actionButton->setStyleSheet("background-color: #48BB78; color: white; font-size: 16px; font-weight: bold; padding: 12px 30px; border-radius: 6px;");
    actionButton->setCursor(Qt::PointingHandCursor);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(infoLabel);
    mainLayout->addWidget(actionButton);

    connect(actionButton, &QPushButton::clicked, this, &TournamentRunnerDialog::runTournamentRound);
}

// 🌟 NEW METHOD: Displayed once all bracket levels are cleared out!
void TournamentRunnerDialog::setupRewardsScreen()
{
    titleLabel = new QLabel("TOURNAMENT COMPLETE!", this);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #D4AF37; background: transparent;");
    titleLabel->setAlignment(Qt::AlignCenter);

    infoLabel = new QLabel(QString("Victory! Your squad conquered the tournament tracks.\n\nReward Claimed: +%1 Gold Pieces")
                               .arg(activeTournament.getReward()),
                           this);
    infoLabel->setStyleSheet("font-size: 16px; color: #E2E8F0; background: transparent;");
    infoLabel->setAlignment(Qt::AlignCenter);

    actionButton = new QPushButton("Collect Rewards & Leave", this);
    actionButton->setStyleSheet("background-color: #3182CE; color: white; font-size: 16px; font-weight: bold; padding: 12px 30px; border-radius: 6px;");
    actionButton->setCursor(Qt::PointingHandCursor);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(infoLabel);
    mainLayout->addWidget(actionButton);

    connect(actionButton, &QPushButton::clicked, this, &QDialog::accept);
}

void TournamentRunnerDialog::clearCurrentLayout()
{
    if (!mainLayout)
        return;

    // 🌟 FIX 1: Explicitly remove widgets from the layout container before deleting them.
    // This stops empty layout wrappers from piling up and breaking window geometry over multiple rounds!
    if (titleLabel)
    {
        mainLayout->removeWidget(titleLabel);
        titleLabel->deleteLater();
        titleLabel = nullptr;
    }
    if (infoLabel)
    {
        mainLayout->removeWidget(infoLabel);
        infoLabel->deleteLater();
        infoLabel = nullptr;
    }
    if (actionButton)
    {
        mainLayout->removeWidget(actionButton);
        actionButton->deleteLater();
        actionButton = nullptr;
    }
}

void TournamentRunnerDialog::runTournamentRound()
{

    // when click start it loads the joust view. After end loads next round runTournament(++currentRound)
    clearCurrentLayout();
    this->show();
    if (!activeTournament.isPlayerParticipating())
    {
        // Player lost and is no longer participatin, open lose screen
    }
    else if (activeTournament.getCurrentRound() > activeTournament.getMaxRounds())
    {
        // Player wins
        setupRewardsScreen();
    }
    else
    {
        QString roundText = QString("ROUND %1").arg(activeTournament.getCurrentRound());

        titleLabel = new QLabel(QString("%1 - %2").arg(QString::fromStdString(activeTournament.getName()), roundText), this);
        titleLabel->setStyleSheet("font-size: 26px; font-weight: bold; color: #F6AD55; background: transparent;");
        titleLabel->setAlignment(Qt::AlignCenter);

        std::string names;
        for (auto k : activeTournament.getEnemyTeam())
        {
            names += " " + k.getName();
        }

        infoLabel = new QLabel(QString("Oponent:%1")
                                   .arg(QString::fromStdString(names)),
                               this);
        infoLabel->setStyleSheet("font-size: 16px; color: #E2E8F0; font-style: italic; background: transparent;");
        infoLabel->setAlignment(Qt::AlignCenter);

        actionButton = new QPushButton("Enter the Arena", this);
        actionButton->setStyleSheet("background-color: #48BB78; color: white; font-size: 16px; font-weight: bold; padding: 12px 30px; border-radius: 6px;");
        actionButton->setCursor(Qt::PointingHandCursor);

        mainLayout->addWidget(titleLabel);
        mainLayout->addWidget(infoLabel);
        mainLayout->addWidget(actionButton);

        connect(actionButton, &QPushButton::clicked, this, &TournamentRunnerDialog::emit nextRoundRequested);
    }
}