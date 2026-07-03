#include "KnightRosterTab.h"
#include "KnightDetailDialog.h"
#include "Player.h"

// Explicitly forward structural attachment rules down to QWidget base constructor
KnightRosterTab::KnightRosterTab(QWidget *parent) : QWidget(parent)
{
    // Apply layout logic directly to THIS object instance!
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *label = new QLabel("YOUR KNIGHTS ROSTER", this);
    label->setStyleSheet("font-size: 20px; font-weight: bold; color: #4A5568;");
    mainLayout->addWidget(label);

    rosterListLayout = new QVBoxLayout();
    mainLayout->addLayout(rosterListLayout);

    mainLayout->addStretch();

    populateRoster();
}

void KnightRosterTab::populateRoster()
{
    // 1. CLEANUP FLUSH: Wipe out any old buttons/widgets currently on screen
    QLayoutItem *child;
    while ((child = rosterListLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater(); // Safely schedule visual deletion
        }
        delete child;
    }

    // 2. REBUILD ROWS: Query the global player data registry
    for (Knight &k : Player::getInstance().getRoster())
    {
        Knight* knightPtr = &k;
        
        QString buttonText = QString("%1 (Manage Stats & Gear)").arg(QString::fromStdString(k.getName()));
        QPushButton *knightButton = new QPushButton(buttonText, this);
        knightButton->setStyleSheet("padding: 12px; font-size: 14px;");
        rosterListLayout->addWidget(knightButton);

        connect(knightButton, &QPushButton::clicked, this, [this, knightPtr]() {
            KnightDetailDialog dialog(*knightPtr, this);
            dialog.exec();
            
            // Optional optimization: If managing equipment can change name layouts,
            // refresh this tab right after the dialog closes:
            this->populateRoster();
        });
    }
}