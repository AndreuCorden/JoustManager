#include "KnightRecruitmentTab.h"
#include "KnightDetailDialog.h"
#include "Player.h"

KnightRecruitmentTab::KnightRecruitmentTab(QWidget *parent) : QWidget(parent)
{
    // Outermost structural window layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *label = new QLabel("RECRUITABLE KNIGHTS", this);
    label->setStyleSheet("font-size: 20px; font-weight: bold; color: #4A5568;");
    mainLayout->addWidget(label);

    // Initialize our empty sub-layout container for the dynamic buttons
    listLayout = new QVBoxLayout();
    mainLayout->addLayout(listLayout);

    mainLayout->addStretch();

    // STEP A: Generate the market data pool EXACTLY ONCE on initialization
    int numRecruitableKnights = 1 + (std::rand() % 3);
    for (int i = 0; i < numRecruitableKnights; ++i)
    {
        recruitableKnights.emplace_back(Knight::generateRandomKnight());
    }

    // STEP B: Trigger the initial rendering pass
    populateList();
}

void KnightRecruitmentTab::populateList()
{
    // 1. CLEANUP FLUSH: Wipe out any old buttons currently on screen
    QLayoutItem *child;
    while ((child = listLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }

    // 2. REBUILD ROWS using iterators instead of indices
    // We use a regular loop but keep tracking the live iterator 'it'
    for (auto it = recruitableKnights.begin(); it != recruitableKnights.end(); ++it)
    {
        // Dereference the iterator to get our specific Knight reference
        Knight &k = *it; 
        
        QString buttonText = QString("%1 (See Stats)").arg(QString::fromStdString(k.getName()));
        QPushButton *knightButton = new QPushButton(buttonText, this);
        knightButton->setStyleSheet("padding: 12px; font-size: 14px;");
        listLayout->addWidget(knightButton);

        // 3. CAPTURE THE ITERATOR BY VALUE: 
        // This gives each individual button a direct handle to its exact node in the list
        connect(knightButton, &QPushButton::clicked, this, [this, it]() {
            
            // Hand the exact knight node over to the profile dialog
            KnightDetailDialog dialog(*it, this);
            
            // 4. INTERCEPT DIALOG ACCEPTANCE
            if (dialog.exec() == QDialog::Accepted) {
                
                // Directly pass the captured iterator to erase it in O(1) time complexity!
                recruitableKnights.erase(it);
                
                // Refresh the interface layout
                populateList();
            }
        });
    }
}