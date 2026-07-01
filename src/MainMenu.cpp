#include "MainMenu.h"

MainMenu::MainMenu(QWidget *parent) : QWidget(parent) {
    // 1. Create the UI elements
    titleLabel = new QLabel("KNIGHT JOUSTING MANAGER", this);
    startButton = new QPushButton("New Game", this);
    exitButton = new QPushButton("Exit to Desktop", this);

    // 2. Style your text (Using simple web styling CSS syntax)
    titleLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: #D4AF37;"); 
    titleLabel->setAlignment(Qt::AlignCenter);

    startButton->setStyleSheet("padding: 10px; font-size: 18px;");
    exitButton->setStyleSheet("padding: 10px; font-size: 18px;");

    // 3. Arrange them vertically using a layout manager
    mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch(); // Pushes elements down
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(40); // Adds clear blank pixels between items
    mainLayout->addWidget(startButton);
    mainLayout->addWidget(exitButton);
    mainLayout->addStretch(); // Pushes elements up (centering the whole group)

    // 4. Hook up button clicks to trigger our game signals using modern C++ lambdas
    connect(startButton, &QPushButton::clicked, this, &MainMenu::startGameRequested);
    connect(exitButton, &QPushButton::clicked, this, &MainMenu::exitRequested);
}