#ifndef _Joust_View
#define _Joust_View

#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QKeyEvent>

#include "Knights/Knight.h"

enum class JoustState
{
    WaitingToStart,
    Charging,
    Clashing,
    Passing,
    MatchOver
};

class JoustView : public QGraphicsView
{
    Q_OBJECT

public:
    // Inside Tournaments/Joust/JoustView.h
    // Modify constructor signature:
    explicit JoustView(const std::vector<Knight> &playerSquad, const std::vector<Knight> &enemySquad, QWidget *parent = nullptr);
    ~JoustView() override;
    void startMatch();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void gameTick();
    void resetRound();
    void triggerQTE();
    void checkQTE(int pressedKey);
    void updateSpriteFrames();

    QGraphicsScene *scene;
    QTimer *gameTimer;
    QTimer *qteTimer;

    // Sprite graphics items
    QGraphicsPixmapItem *playerSprite;
    QGraphicsPixmapItem *enemySprite;
    QGraphicsLineItem *tiltBarrier;
    QGraphicsTextItem *qteDisplayItem;
    QGraphicsTextItem *announcerText;

    // Game logic states
    JoustState currentState;
    int currentCharge; // Tracks 1, 2, or 3
    int targetQteKey;  // The active Qt::Key the player must press
    bool qteActive;

    // Kinematics / Position tracking variables
    qreal playerX, enemyX;
    qreal baseSpeed;
    qreal playerSpeedBoost;
    int spriteFrameCounter;
    int currentFrameIndex;

    QPixmap spriteSheet;

    int playerHorseColorIdx;
    int enemyHorseColorIdx;

    enum class ViewDirection {
        Down = 0,
        Left = 1,
        Right = 2,
        Up = 3
    };

    ViewDirection playerFacing;
    ViewDirection enemyFacing;

    QPixmap getHorseFrame(int breedIndex, ViewDirection direction, int frameNum);
    void updateVisualSprites();
};

#endif