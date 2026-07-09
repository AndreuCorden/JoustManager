#include "view/knights/KnightRecruitmentView.h"
#include "model/knights/Knight.h"
#include "view/knights/KnightDetailDialog.h"


KnightRecruitmentView::KnightRecruitmentView(Player &player, QWidget *parent)
    : QWidget(parent)
    , m_player(player)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *label = new QLabel("RECRUITABLE KNIGHTS", this);
    label->setStyleSheet("font-size: 20px; font-weight: bold; color: #4A5568;");
    mainLayout->addWidget(label);

    // Initialize our empty sub-layout container for the dynamic buttons
    listLayout = new QVBoxLayout();
    mainLayout->addLayout(listLayout);

    mainLayout->addStretch();
}

void KnightRecruitmentView::populateList(const std::list<Knight> &recruitableKnights)
{
    QLayoutItem *child;
    while ((child = listLayout->takeAt(0)) != nullptr)
    {
        if (child->widget())
        {
            child->widget()->deleteLater();
        }
        delete child;
    }

    // 2. REBUILD ROWS using iterators instead of indices
    for (const Knight &k : recruitableKnights)
    {
        QString buttonText = QString("%1 (See Stats)").arg(QString::fromStdString(k.getName()));
        QPushButton *knightButton = new QPushButton(buttonText, this);
        knightButton->setStyleSheet("padding: 12px; font-size: 14px;");
        listLayout->addWidget(knightButton);

        // 3. CAPTURE THE ITERATOR BY VALUE:
        // This gives each individual button a direct handle to its exact node in the list
        connect(knightButton, &QPushButton::clicked, this, [this, k]()
                {
            
            // Hand the exact knight node over to the profile dialog
            KnightDetailDialog dialog(m_player, k, this);
            
            // 4. INTERCEPT DIALOG ACCEPTANCE
            if (dialog.exec() == QDialog::Accepted) {
                
                emit knightRecruited(k);
                
            } });
    }
}