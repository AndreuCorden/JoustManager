#ifndef _Main_Menu
#define _Main_Menu

#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class MainMenu : public QWidget {
    Q_OBJECT // Required macro for Qt custom event systems

public:
    explicit MainMenu(QWidget *parent = nullptr);

signals:
    // We define custom C++ signals that other parts of our game can listen to
    void startGameRequested();
    void exitRequested();

private:
    QLabel *titleLabel;
    QPushButton *startButton;
    QPushButton *exitButton;
    QVBoxLayout *mainLayout;
};

#endif