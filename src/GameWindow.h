#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#pragma once

#include <QMainWindow>
#include <QKeyEvent>
#include <QStackedWidget>
#include "MainMenu.h"
#include "ManagerInterface.h"

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