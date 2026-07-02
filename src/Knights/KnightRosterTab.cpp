#include "KnightRosterTab.h"

// Explicitly forward structural attachment rules down to QWidget base constructor
KnightRosterTab::KnightRosterTab(QWidget *parent) : QWidget(parent)
{
    // Apply layout logic directly to THIS object instance!
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel("YOUR KNIGHTS ROSTER", this);
    label->setStyleSheet("font-size: 20px; font-weight: bold; color: #4A5568;");

    layout->addWidget(label);

    Knight knight = Knight::generateRandomKnight();
    addKnightToRosterTab(knight);

    for (Knight k : knightsInRoster)
    {
        // Use .arg() to cleanly inject your C++ string into a Qt string layout
        QString buttonText = QString("%1 (Manage Stats & Gear)").arg(QString::fromStdString(k.getName()));

        QPushButton *knightButton = new QPushButton(buttonText, this);
        knightButton->setStyleSheet("padding: 12px; font-size: 14px;");
        layout->addWidget(knightButton);
    }

    layout->addStretch();
}

void KnightRosterTab::addKnightToRosterTab(const Knight &knigth)
{
    knightsInRoster.emplace_back(knigth);
}