#include "Tournaments/TournamentTab.h"
#include "TournamentManager.h"
#include "KnightSelectionDialog.h"

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
    // Wipe out old row assets cleanly
    QLayoutItem *child;
    while ((child = tournamentListLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    auto& activeTourneys = TournamentManager::getInstance().getActiveTournaments();

    for (size_t i = 0; i < activeTourneys.size(); ++i) {
        Tournament &t = activeTourneys[i];

        QWidget *rowWidget = new QWidget(this);
        QHBoxLayout *rowLayout = new QHBoxLayout(rowWidget);

        QLabel *nameLabel = new QLabel(QString::fromStdString(t.getName()), this);
        nameLabel->setStyleSheet("font-size: 15px; font-weight: bold; color: white;");
        rowLayout->addWidget(nameLabel);
        rowLayout->addStretch();

        // Branch UI behavior based on whether team selection array holds data
        if (!t.hasTeamAssembled()) {
            QPushButton *enterBtn = new QPushButton(QString("Enter (%1v%1)").arg(t.getRequiredTeammates()), this);
            enterBtn->setStyleSheet("padding: 10px 20px; background-color: #D4AF37; color: black; font-weight: bold;");
            rowLayout->addWidget(enterBtn);

            connect(enterBtn, &QPushButton::clicked, this, [this, &t]() {
                KnightSelectionDialog selectDialog(t, this);
                if (selectDialog.exec() == QDialog::Accepted) {
                    populateRoster(); // Refresh screen state layouts 
                }
            });
        } else {
            // Action changes to active simulation launch state!
            QPushButton *playBtn = new QPushButton("⚔️ PLAY MATCH", this);
            playBtn->setStyleSheet("padding: 10px 20px; background-color: #2F855A; color: white; font-weight: bold;");
            rowLayout->addWidget(playBtn);

            QPushButton *resetBtn = new QPushButton("Reset Squad", this);
            resetBtn->setStyleSheet("padding: 10px; background-color: #E53E3E; color: white; font-size: 11px;");
            rowLayout->addWidget(resetBtn);

            connect(playBtn, &QPushButton::clicked, this, [this, &t]() {
                // Placeholder area: Hook up your combat visualizer engine here!
                QMessageBox info(this);
                info.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
                info.setText("CRITICAL NOTE: Playing simulation animation right now!");
                info.exec();
            });

            connect(resetBtn, &QPushButton::clicked, this, [this, &t]() {
                t.clearTeam();
                populateRoster();
            });
        }

        tournamentListLayout->addWidget(rowWidget);
    }
}