#ifndef JOUSTHANDLER_H
#define JOUSTHANDLER_H

#include <vector>
#include "model/knights/Knight.h"

enum class JoustState {
    WaitingToStart,
    Charging,
    Clashing,
    Passing,
    MatchOver
};

class JoustModel
{
public:
    JoustModel();

    void startMatch();
    void resetRound();
    void updateTick(int sceneWidth);
    void checkQTE(int pressedKey);
    void triggerQTE(int targetKey, const std::string& keyChar);

    // Getters
    JoustState getCurrentState() const { return m_currentState; }
    int getCurrentCharge() const { return m_currentCharge; }
    double getPlayerX() const { return m_playerX; }
    double getEnemyX() const { return m_enemyX; }
    bool isQteActive() const { return m_qteActive; }
    std::string getQteText() const { return m_qteText; }
    std::string getAnnouncerText() const { return m_announcerText; }
    int getCurrentFrameIndex() const { return m_currentFrameIndex; }
    void advanceAnimationFrame() { m_currentFrameIndex = (m_currentFrameIndex + 1) % 6; }

private:
    JoustState m_currentState;
    int m_currentCharge;
    int m_targetQteKey;
    bool m_qteActive;

    double m_playerX;
    double m_enemyX;
    double m_baseSpeed;
    double m_playerSpeedBoost;
    int m_currentFrameIndex;

    std::string m_qteText;
    std::string m_announcerText;
};

#endif