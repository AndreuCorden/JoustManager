#ifndef JOUSTVIEW_H
#define JOUSTVIEW_H

#pragma once
#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include "model/tournaments/JoustHandler.h"

class JoustView : public QWidget
{
    Q_OBJECT
public:
    explicit JoustView(QWidget *parent = nullptr);
    
    void updateRenderState(double pX, double eX, int charge, int frameIndex, 
                           JoustState state, const QString& announcer, const QString& qte);

    void setScoutingReport(const QString& opponentName, const QString& traitAdvice);

signals:
    void actionTriggered(); // Captures click or Space bar setups
    void inputKeyPressed(int key);
    void viewClosed();
    void strategySelected(int stanceIndex);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    

private:
    enum class ViewDirection { Left, Right };
    QPixmap getHorseFrame(int colorIdx, ViewDirection dir, int frameNum);
    QPixmap getKnightPixmap(ViewDirection dir, int frameIndex);

    QGraphicsView *graphicsView;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *playerSprite;
    QGraphicsPixmapItem *enemySprite;
    QGraphicsPixmapItem *playerKnightSprite;
    QGraphicsPixmapItem *enemyKnightSprite;
    QGraphicsPixmapItem *tiltBarrier;

    QLabel *scoutingTextLabel;
    QWidget *strategyContainer;

    QPixmap horseSheets[4];
    QPixmap knightPixmap;
};

#endif