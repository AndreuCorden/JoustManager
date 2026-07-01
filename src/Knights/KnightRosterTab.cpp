#include "KnightRosterTab.h"

// Explicitly forward structural attachment rules down to QWidget base constructor
KnightRosterTab::KnightRosterTab(QWidget *parent) : QWidget(parent)
{
    // Apply layout logic directly to THIS object instance!
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel("YOUR KNIGHTS ROSTER", this);
    label->setStyleSheet("font-size: 20px; font-weight: bold; color: #4A5568;");

    QPushButton *knightButton = new QPushButton("Sir Lancelot (Manage Stats & Gear)", this);
    knightButton->setStyleSheet("padding: 12px; font-size: 14px;");

    layout->addWidget(label);
    layout->addWidget(knightButton);
    layout->addStretch();
}