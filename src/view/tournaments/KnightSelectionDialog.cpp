// Tournaments/KnightSelectionDialog.cpp
#include "KnightSelectionDialog.h"
#include "Player.h"
#include <QLabel>
#include <QPushButton>

KnightSelectionDialog::KnightSelectionDialog(Player &player, const Tournament &tournament, QWidget *parent) 
    : GameDialog(parent)
    , m_requiredTeammates(tournament.getRequiredTeammates()) 
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setModal(true);
    setMinimumSize(400, 450);

    QLabel *title = new QLabel(QString::fromStdString(tournament.getName()), this);
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #D4AF37;");
    mainLayout->addWidget(title);

    QLabel *ruleLabel = new QLabel(QString("Select exactly %1 Knight(s) to deploy:").arg(m_requiredTeammates), this);
    ruleLabel->setStyleSheet("font-size: 13px; color: #A0AEC0; margin-bottom: 10px;");
    mainLayout->addWidget(ruleLabel);

    // Loop through player's available combatants
    auto& roster = player.getRoster();
    if (roster.empty()) {
        QLabel *emptyLabel = new QLabel("You have no knights in your roster! Recruit some first.", this);
        emptyLabel->setWordWrap(true);
        mainLayout->addWidget(emptyLabel);
    }

    for (Knight &k : roster) {
        QHBoxLayout *row = new QHBoxLayout();
        QCheckBox *box = new QCheckBox(this);
        QLabel *name = new QLabel(QString::fromStdString(k.getName()), this);
        name->setStyleSheet("font-size: 14px; color: white;");

        row->addWidget(box);
        row->addWidget(name);
        row->addStretch();
        mainLayout->addLayout(row);

        // Keep track of which checkbox maps to which internal Knight pointer
        checkboxRegistry.push_back({&k, box});
    }

    mainLayout->addStretch();

    // Confirm deployment action button
    QPushButton *confirmBtn = new QPushButton("DEPLOY SQUAD", this);
    confirmBtn->setStyleSheet("padding: 12px; background-color: #2B6CB0; color: white; font-weight: bold;");
    mainLayout->addWidget(confirmBtn);

    connect(confirmBtn, &QPushButton::clicked, this, [this]() {
        m_selectedKnights.clear();
        
        for (auto &pair : checkboxRegistry) {
            if (pair.second->isChecked()) {
                m_selectedKnights.push_back(pair.first);
            }
        }

        // Validate squad size restrictions
        if (m_selectedKnights.size() != m_requiredTeammates) {
            QMessageBox alert(this);
            alert.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
            alert.setText(QString("Invalid deployment size! You must select exactly %1 knights.")
                          .arg(m_requiredTeammates));
            alert.exec();
            return;
        }
        this->accept();
    });

    QPushButton *cancelBtn = new QPushButton("CANCEL", this);
    cancelBtn->setStyleSheet("background: transparent; color: #718096;");
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    mainLayout->addWidget(cancelBtn);
}