#include "view/tournaments/TournamentView.h"
#include "view/tournaments/KnightSelectionDialog.h"

TournamentView::TournamentView(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *label = new QLabel("UPCOMING TOURNAMENTS", this);
    label->setStyleSheet("font-size: 20px; font-weight: bold; color: #4A5568;");
    mainLayout->addWidget(label);

    m_listLayout = new QVBoxLayout();
    mainLayout->addLayout(m_listLayout);
    mainLayout->addStretch();
}

void TournamentView::populateRoster(std::vector<Tournament> const &todaysTournaments)
{
    // Clean layout views out completely
    QLayoutItem *child;
    while ((child = m_listLayout->takeAt(0)) != nullptr)
    {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }

    for (size_t i = 0; i < todaysTournaments.size(); ++i)
    {
        const Tournament &t = todaysTournaments[i];

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

            connect(enterBtn, &QPushButton::clicked, this, [this, i, t]()
                    {
            KnightSelectionDialog selectDialog(t, this);
            if (selectDialog.exec() == QDialog::Accepted) {
                std::vector<Knight> squad = selectDialog.getSelectedKnights();
                emit registrationRequested(i, squad);
            } });
        }
        // STATE B: Tournament is actively running its structural round passes
        else
        {
            QPushButton *leaveBtn = new QPushButton("Leave Torunament", this);
            leaveBtn->setStyleSheet("padding: 10px; background-color: #E53E3E; color: white;");
            rowLayout->addWidget(leaveBtn);

            connect(leaveBtn, &QPushButton::clicked, this, [this, i]()
                    { emit cancellationRequested(i); });
        }

        m_listLayout->addWidget(rowWidget);
    }
}