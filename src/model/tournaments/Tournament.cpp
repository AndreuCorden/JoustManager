// Tournaments/Tournament.cpp
#include <QRandomGenerator>

#include "Tournament.h"

void Tournament::initializeTournament()
{
    m_activeTeams.clear();
    m_currentRound = 1;
    m_playerParticipating = true;

    // 1. Add player squad first
    m_activeTeams.push_back(m_playerTeam);

    // 2. Compute total participants using your math formula: 2^maxRounds
    int totalTeamsNeeded = std::pow(2, m_maxRounds);

    // Fill the rest of the bracket with procedurally generated bots
    for (int i = 1; i < totalTeamsNeeded; ++i)
    {
        std::vector<Knight> botTeam;
        for (int t = 0; t < m_teammates; ++t)
        {
            botTeam.push_back(Knight("Sir Placeholder", "Camelot", "Title", 180, 95, 1, 200
                                     , 0, 0, 0, 0, 0, 0, 0, 0));
        }
        m_activeTeams.push_back(botTeam);
    }
}

std::vector<Knight> Tournament::getEnemyTeam() const
{
    if (m_activeTeams.empty())
    {
        return {Knight("Sir Placeholder", "Camelot", "Title", 180, 95, 1, 200
        , 0, 0, 0, 0, 0, 0, 0, 0)};
    }

    auto *rand = QRandomGenerator::global();
    size_t randomIndex = rand->bounded(m_activeTeams.size());

    return m_activeTeams[randomIndex];
}

void Tournament::advanceTournamentRound(const bool won)
{
    m_playerParticipating = won;
    if (!m_playerParticipating)
    {
        // simulate rest of tournament
    }
    else
    {
        m_currentRound++;
    }
}

void Tournament::endTournament(const bool won)
{
    int num_rounds = m_currentRound;
    if(!won)
    {
        --m_currentRound;
    }

    for(auto k : m_playerTeam)
    {
        k.gainXP(m_expPerRound * num_rounds);
    }
}