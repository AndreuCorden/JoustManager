#include "GameWindow.h"
#include <QLabel>

GameWindow::GameWindow()
{

    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);

    this->showFullScreen();
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

void GameWindow::keyPressEvent(QKeyEvent *event) {
    // Check if the key that was pressed is the Escape key
    if (event->key() == Qt::Key_Escape) {
        this->close(); // Cleanly shuts down the entire application
    } else {
        // Pass any other keypresses back to the standard engine
        QMainWindow::keyPressEvent(event);
    }
}