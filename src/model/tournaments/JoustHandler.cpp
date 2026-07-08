#include "model/tournaments/JoustHandler.h"

JoustModel::JoustModel()
    : m_currentState(JoustState::WaitingToStart), m_currentCharge(1)
    , m_qteActive(false), m_baseSpeed(6.0), m_playerSpeedBoost(0.0)
    , m_currentFrameIndex(0), m_announcerText("READY TO JOUST!\nClick Screen or Press SPACE to Charge")
{}

void JoustModel::startMatch() {
    if (m_currentState == JoustState::WaitingToStart) {
        m_currentState = JoustState::Charging;
        m_announcerText = "PASS " + std::to_string(m_currentCharge) + "! CHARGE!";
    }
}

void JoustModel::resetRound() {
    m_qteActive = false;
    m_qteText = "";
    m_playerSpeedBoost = 0.0;
    // Set starting positions based on odd/even charges
    m_playerX = (m_currentCharge % 2 != 0) ? 50.0 : 1050.0;
    m_enemyX = (m_currentCharge % 2 != 0) ? 1050.0 : 50.0;
}

void JoustModel::triggerQTE(int targetKey, const std::string& keyChar) {
    if (m_currentState != JoustState::Charging) return;
    m_targetQteKey = targetKey;
    m_qteText = keyChar;
    m_qteActive = true;
}

void JoustModel::checkQTE(int pressedKey) {
    if (!m_qteActive) return;
    if (pressedKey == m_targetQteKey) {
        m_playerSpeedBoost += 2.5;
        m_qteText = "BOOST!";
    } else {
        m_qteText = "MISS!";
    }
    m_qteActive = false;
}

void JoustModel::updateTick(int sceneWidth) {
    if (m_currentState != JoustState::Charging && m_currentState != JoustState::Passing) return;

    double pDirection = (m_currentCharge % 2 != 0) ? 1.0 : -1.0;
    double eDirection = -pDirection;

    m_playerX += (m_baseSpeed + m_playerSpeedBoost) * pDirection;
    m_enemyX += m_baseSpeed * eDirection;

    if (m_currentState == JoustState::Charging) {
        if ((pDirection > 0 && m_playerX >= m_enemyX) || (pDirection < 0 && m_playerX <= m_enemyX)) {
            m_currentState = JoustState::Passing;
            m_qteText = "💥 SMASH!";
        }
    }

    bool playerFinished = (pDirection > 0) ? (m_playerX > sceneWidth) : (m_playerX < -50);
    if (playerFinished) {
        if (m_currentCharge < 3) {
            m_currentCharge++;
            m_currentState = JoustState::WaitingToStart;
            m_announcerText = "Pass " + std::to_string(m_currentCharge - 1) + " Completed.\nPress SPACE for next Pass!";
            resetRound();
        } else {
            m_currentState = JoustState::MatchOver;
            m_announcerText = "VICTORY!\n\nClick Screen to Complete Match.";
            m_qteText = "";
        }
    }
}