#include "KnightRecruitmentTab.h"
#include "KnightDetailDialog.h"
#include "Player.h"

// Explicitly forward structural attachment rules down to QWidget base constructor
KnightRecruitmentTab::KnightRecruitmentTab(QWidget *parent) : QWidget(parent)
{
    // Apply layout logic directly to THIS object instance!
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel("RECRUITABLE KNIGHTS", this);
    label->setStyleSheet("font-size: 20px; font-weight: bold; color: #4A5568;");

    layout->addWidget(label);

    int numRecruitableKnights = 1 + (std::rand() % 3);

    for (int i = 0; i < numRecruitableKnights; ++i)
    {
        recruitableKnights.emplace_back(Knight::generateRandomKnight());
        Knight k = recruitableKnights.back();
        QString buttonText = QString("%1 (See Stats)").arg(QString::fromStdString(k.getName()));
        QPushButton *knightButton = new QPushButton(buttonText, this);
        knightButton->setStyleSheet("padding: 12px; font-size: 14px;");
        layout->addWidget(knightButton);

        // 3. Pass the pointer into the lambda by value
        connect(knightButton, &QPushButton::clicked, this, [this, k]() {
            // Dereference the pointer (*) to hand it to the dialog
            KnightDetailDialog dialog(recruitableKnights.back(), this);
            dialog.exec();
        });
    }

    layout->addStretch();
}