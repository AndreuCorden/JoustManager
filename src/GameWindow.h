#ifndef _Game_Window
#define _Game_Window

#include "ManagerInterface.h"

#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include "MainMenu.h"

class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    GameWindow();

private slots:
    void handleStartGame();

private:
    QStackedWidget *screenContainer;
    MainMenu *mainMenu;
    ManagerInterface *managerInterface;
};

#endif