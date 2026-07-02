#include "GameWindow.h"
#include <QLabel>

GameWindow::GameWindow()
{

    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);

    setMinimumSize(1024, 768);
    setWindowTitle("Knight Jousting Manager");

    screenContainer = new QStackedWidget(this);
    setCentralWidget(screenContainer);

    // Instantiate your main menu screen
    mainMenu = new MainMenu(this);
    screenContainer->addWidget(mainMenu);

    managerInterface = new ManagerInterface(this);
    screenContainer->addWidget(managerInterface);

    // Listen to the menu signals
    connect(mainMenu, &MainMenu::startGameRequested, this, &GameWindow::handleStartGame);
    connect(mainMenu, &MainMenu::exitRequested, this, &GameWindow::close);

    // Show the menu first
    screenContainer->setCurrentWidget(mainMenu);
}

void GameWindow::handleStartGame()
{
    // Switch the screen inside the container seamlessly
    screenContainer->setCurrentWidget(managerInterface);
}