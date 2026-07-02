#ifndef _Game_Window
#define _Game_Window

#include "ManagerInterface.h"

#pragma once

#include <QMainWindow>
#include <QKeyEvent>
#include <QStackedWidget>
#include "MainMenu.h"

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    GameWindow();

private slots:
    void handleStartGame();

private:
    QStackedWidget *screenContainer;
    MainMenu *mainMenu;
    ManagerInterface *managerInterface;

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif