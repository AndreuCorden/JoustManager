#include "Tournaments/TournamentTab.h"
#include "Tournaments/TournamentManager.h"
#include "Tournaments/KnightSelectionDialog.h"
#include "Tournaments/Joust/JoustView.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDialog>

TournamentTab::TournamentTab(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *label = new QLabel("UPCOMING TOURNAMENTS", this);
    label->setStyleSheet("font-size: 20px; font-weight: bold; color: #4A5568;");
    mainLayout->addWidget(label);

    tournamentListLayout = new QVBoxLayout();
    mainLayout->addLayout(tournamentListLayout);
    mainLayout->addStretch();

    populateRoster();
}

void TournamentTab::populateRoster()
{
    // Clean layout views out completely
    QLayoutItem *child;
    while ((child = tournamentListLayout->takeAt(0)) != nullptr)
    {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }

    auto &activeTourneys = TournamentManager::getInstance().getActiveTournaments();

    for (size_t i = 0; i < activeTourneys.size(); ++i)
    {
        Tournament &t = activeTourneys[i];

        QWidget *rowWidget = new QWidget(this);
        QHBoxLayout *rowLayout = new QHBoxLayout(rowWidget);

        QLabel *nameLabel = new QLabel(QString::fromStdString(t.getName()), this);
        nameLabel->setStyleSheet("font-size: 15px; font-weight: bold; color: white;");
        rowLayout->addWidget(nameLabel);
        rowLayout->addStretch();

        // STATE A: Player hasn't joined up yet
        if (!t.isActive())
        {
            QPushButton *enterBtn = new QPushButton(QString("Enter Tournament (%1v%1)").arg(t.getRequiredTeammates()), this);
            enterBtn->setStyleSheet("padding: 10px 20px; background-color: #D4AF37; color: black; font-weight: bold;");
            rowLayout->addWidget(enterBtn);

            connect(enterBtn, &QPushButton::clicked, this, [this, i]()
                    {
                auto &tourneys = TournamentManager::getInstance().getActiveTournaments();
                Tournament &targetTourney = tourneys[i];

                KnightSelectionDialog selectDialog(targetTourney, this);
                if (selectDialog.exec() == QDialog::Accepted) {
                    populateRoster(); 
                } });
        }
        // STATE B: Tournament is actively running its structural round passes
        else
        {
            QString processText = QString("⚔️ RUN ROUND %1 / %2").arg(t.getCurrentRound()).arg(t.getMaxRounds());
            QPushButton *playBtn = new QPushButton(processText, this);
            playBtn->setStyleSheet("padding: 10px 20px; background-color: #2F855A; color: white; font-weight: bold;");
            rowLayout->addWidget(playBtn);

            QPushButton *forfeitBtn = new QPushButton("Forfeit", this);
            forfeitBtn->setStyleSheet("padding: 10px; background-color: #E53E3E; color: white;");
            rowLayout->addWidget(forfeitBtn);

            connect(playBtn, &QPushButton::clicked, this, [this, i]()
                    {
                auto &tourneys = TournamentManager::getInstance().getActiveTournaments();
                Tournament &targetTourney = tourneys[i];

                std::vector<MatchUp> currentMatches = targetTourney.generateCurrentRoundMatches();
                std::vector<std::vector<Knight>> roundWinners;
                bool playerSurvivedRound = false;

                if (currentMatches.empty()) return;

                // Track if we already launched an interactive arena window for the player
                bool playerMatchHandled = false;

                for (const MatchUp &match : currentMatches)
                {
                    if (match.involvesPlayer)
                    {
                        // 👍 FIXED: Guard against opening duplicate or runaway windows in the loop
                        if (playerMatchHandled) {
                            // If player data was duplicated, let background simulation process it safely
                            roundWinners.push_back(match.teamA);
                            continue;
                        }

                        QDialog arenaWindow(this);
                        arenaWindow.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
                        arenaWindow.setModal(true);
                        QVBoxLayout *layout = new QVBoxLayout(&arenaWindow);

                        JoustView *joustArena = new JoustView(match.teamA, match.teamB, &arenaWindow);
                        layout->addWidget(joustArena);

                        joustArena->setFocusPolicy(Qt::StrongFocus);
                        joustArena->setFocus();

                        QPushButton *exitBtn = new QPushButton("Return to Base", &arenaWindow);
                        exitBtn->setStyleSheet("padding: 8px; font-weight: bold;");
                        connect(exitBtn, &QPushButton::clicked, &arenaWindow, &QDialog::accept);
                        layout->addWidget(exitBtn);

                        arenaWindow.exec(); // Blocks execution thread cleanly here

                        roundWinners.push_back(match.teamA);
                        playerSurvivedRound = true;
                        playerMatchHandled = true; // Mark as done so it cannot loop back open
                    }
                    else
                    {
                        roundWinners.push_back(match.teamA);
                    }
                }

                targetTourney.advanceTournamentRound(roundWinners);

                if (!targetTourney.isActive() && playerSurvivedRound)
                {
                    targetTourney.clearTournament();
                }

                this->populateRoster(); });

            connect(forfeitBtn, &QPushButton::clicked, this, [this, i]()
                    {
                auto &tourneys = TournamentManager::getInstance().getActiveTournaments();
                tourneys[i].clearTournament();
                this->populateRoster(); });
        }

        tournamentListLayout->addWidget(rowWidget);
    }
}