#ifndef JOUSTCONTROLLER_H
#define JOUSTCONTROLLER_H

#pragma once
#include <QObject>
#include <QTimer>
#include "model/tournaments/JoustHandler.h"
#include "view/tournaments/JoustView.h"

class JoustController : public QObject
{
    Q_OBJECT
public:
    explicit JoustController(QObject *parent = nullptr);
    ~JoustController() override;

    void startMatchLoop();
    JoustView* getView() { return mView; }

signals:
    void matchFinished();

private slots:
    void onPhysicsTick();
    void onQteTick();
    void handleAction();
    void handleKeyPress(int key);

private:
    JoustModel *mModel;
    JoustView *mView;
    QTimer *mPhysicsTimer;
    QTimer *mQteTimer;
    int mFrameCounter;
};

#endif