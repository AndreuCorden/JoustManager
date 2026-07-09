#ifndef JOUSTVIEW_H
#define JOUSTVIEW_H

#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include "model/tournaments/JoustHandler.h"

class JoustView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit JoustView(QWidget *parent = nullptr);
    
    void updateRenderState(double pX, double eX, int charge, int frameIndex, 
                           JoustState state, const QString& announcer, const QString& qte);

signals:
    void actionTriggered(); // Captures click or Space bar setups
    void inputKeyPressed(int key);
    void viewClosed();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    enum class ViewDirection { Left, Right };
    QPixmap getHorseFrame(int colorIdx, ViewDirection dir, int frameNum);
    QPixmap getKnightPixmap(ViewDirection dir, int frameIndex);

    QGraphicsScene *scene;
    QGraphicsPixmapItem *playerSprite;
    QGraphicsPixmapItem *enemySprite;
    QGraphicsPixmapItem *playerKnightSprite;
    QGraphicsPixmapItem *enemyKnightSprite;
    QGraphicsPixmapItem *tiltBarrier;
    QGraphicsTextItem *qteDisplayItem;
    QGraphicsTextItem *announcerText;

    QPixmap horseSheets[4];
    QPixmap knightPixmap;
};

#endif