#include "Tournaments/TournamentTab.h"

TournamentTab::TournamentTab(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *label = new QLabel("UPCOMING TOURNAMENTS", this);
    label->setStyleSheet("font-size: 20px; font-weight: bold; color: #4A5568;");

    tournamentListLayout = new QVBoxLayout();
    mainLayout->addLayout(tournamentListLayout);

    mainLayout->addWidget(label);
    mainLayout->addStretch();

    populateRoster();
}

void TournamentTab::populateRoster()
{
    // 1. CLEANUP FLUSH: Wipe out any old buttons/widgets currently on screen
    QLayoutItem *child;
    while ((child = tournamentListLayout->takeAt(0)) != nullptr)
    {
        if (child->widget())
        {
            child->widget()->deleteLater(); // Safely schedule visual deletion
        }
        delete child;
    }

    QPushButton *enterTourney = new QPushButton("Enter the King's Grand Tournament", this);
    enterTourney->setStyleSheet("padding: 15px; background-color: #D4AF37; color: black; font-weight: bold;");
    tournamentListLayout->addWidget(enterTourney);
}