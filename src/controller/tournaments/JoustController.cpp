#include "JoustController.h"
#include <QRandomGenerator>

JoustController::JoustController(QObject *parent)
    : QObject(parent), mModel(new JoustModel()), mView(new JoustView()), mFrameCounter(0)
{
    mPhysicsTimer = new QTimer(this);
    mQteTimer = new QTimer(this);

    connect(mPhysicsTimer, &QTimer::timeout, this, &JoustController::onPhysicsTick);
    connect(mQteTimer, &QTimer::timeout, this, &JoustController::onQteTick);

    connect(mView, &JoustView::actionTriggered, this, &JoustController::handleAction);
    connect(mView, &JoustView::inputKeyPressed, this, &JoustController::handleKeyPress);
    connect(mView, &JoustView::viewClosed, this, &JoustController::matchFinished);

    mModel->resetRound();
    onPhysicsTick(); // Initial render setup pass
}

JoustController::~JoustController() {
    mPhysicsTimer->stop();
    mQteTimer->stop();
    delete mModel;
    // mView is deleted automatically if treated as a sub-widget window, or handled via parent rules.
}

void JoustController::startMatchLoop() {
    mPhysicsTimer->start(16);
}

void JoustController::handleAction() {
    if (mModel->getCurrentState() == JoustState::WaitingToStart) {
        mModel->startMatch();
        mQteTimer->start(800);
    } else if (mModel->getCurrentState() == JoustState::MatchOver) {
        mView->close();
    }
}

void JoustController::handleKeyPress(int key) {
    if (mModel->isQteActive()) {
        mModel->checkQTE(key);
    }
}

void JoustController::onQteTick() {
    if (mModel->getCurrentState() != JoustState::Charging) return;
    
    int randIdx = QRandomGenerator::global()->bounded(4);
    const char keys[] = {'W', 'A', 'S', 'D'};
    const int qtKeys[] = {Qt::Key_W, Qt::Key_A, Qt::Key_S, Qt::Key_D};

    mModel->triggerQTE(qtKeys[randIdx], std::string(1, keys[randIdx]));
}

void JoustController::onPhysicsTick() {
    mModel->updateTick(1200);

    if (mModel->getCurrentState() == JoustState::Passing) {
        mQteTimer->stop();
    }

    if (mModel->getCurrentState() == JoustState::Charging || mModel->getCurrentState() == JoustState::Passing) {
        mFrameCounter++;
        if (mFrameCounter >= 5) {
            mFrameCounter = 0;
            mModel->advanceAnimationFrame();
        }
    }

    mView->updateRenderState(
        mModel->getPlayerX(), mModel->getEnemyX(), mModel->getCurrentCharge(),
        mModel->getCurrentFrameIndex(), mModel->getCurrentState(),
        QString::fromStdString(mModel->getAnnouncerText()), QString::fromStdString(mModel->getQteText())
    );
}