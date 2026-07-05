#include "Tournaments/TournamentTab.h"
#include "Tournaments/KnightSelectionDialog.h"
#include "Tournaments/Joust/JoustView.h"
#include "GameTimelineController.h"
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

    auto &tourneys = GameTimelineController::getInstance().getAvailableTournaments();

    for (size_t i = 0; i < tourneys.size(); ++i)
    {
        Tournament &t = tourneys[i];

        QWidget *rowWidget = new QWidget(this);
        QHBoxLayout *rowLayout = new QHBoxLayout(rowWidget);

        QLabel *nameLabel = new QLabel(QString::fromStdString(t.getName()), this);
        nameLabel->setStyleSheet("font-size: 15px; font-weight: bold; color: white;");
        rowLayout->addWidget(nameLabel);
        rowLayout->addStretch();

        // STATE A: Player hasn't joined up yet
        if (!t.isPlayerParticipating())
        {
            QPushButton *enterBtn = new QPushButton(QString("Enter Tournament (%1v%1)").arg(t.getRequiredTeammates()), this);
            enterBtn->setStyleSheet("padding: 10px 20px; background-color: #D4AF37; color: black; font-weight: bold;");
            rowLayout->addWidget(enterBtn);

            connect(enterBtn, &QPushButton::clicked, this, [this, i]()
                    {
            auto &liveTourneys = GameTimelineController::getInstance().getAvailableTournaments();
            Tournament &targetTourney = liveTourneys[i];

            KnightSelectionDialog selectDialog(targetTourney, this);
            if (selectDialog.exec() == QDialog::Accepted) {
                GameTimelineController::getInstance().registerForTournament(i);
                populateRoster(); 
            } });
        }
        // STATE B: Tournament is actively running its structural round passes
        else
        {
            QPushButton *leaveBtn = new QPushButton("Leave Torunament", this);
            leaveBtn->setStyleSheet("padding: 10px; background-color: #E53E3E; color: white;");
            rowLayout->addWidget(leaveBtn);

            connect(leaveBtn, &QPushButton::clicked, this, [this, i]()
                    {
    auto &liveTourneys = GameTimelineController::getInstance().getAvailableTournaments();
    Tournament &t = liveTourneys[i];

    GameTimelineController::getInstance().cancelTournamentRegistration(t.getName());
    t.clearTournament(); 
    this->populateRoster(); });
        }

        tournamentListLayout->addWidget(rowWidget);
    }
}