// Tournaments/Tournament.cpp
#include <QRandomGenerator>

#include "Tournament.h"

void Tournament::initializeTournament()
{
    activeTeams.clear();
    currentRound = 1;
    playerParticipating = true;

    // 1. Add player squad first
    activeTeams.push_back(playerTeam);

    // 2. Compute total participants using your math formula: 2^maxRounds
    int totalTeamsNeeded = std::pow(2, maxRounds);

    // Fill the rest of the bracket with procedurally generated bots
    for (int i = 1; i < totalTeamsNeeded; ++i)
    {
        std::vector<Knight> botTeam;
        for (int t = 0; t < teammates; ++t)
        {
            botTeam.push_back(Knight("Sir Placeholder", "Camelot", "Title", 180, 95, 200));
        }
        activeTeams.push_back(botTeam);
    }
}

std::vector<Knight> Tournament::getEnemyTeam() const
{
    if (activeTeams.empty())
    {
        return {Knight("Sir Placeholder", "Camelot", "Title", 180, 95, 200)};
    }

    auto *rand = QRandomGenerator::global();
    size_t randomIndex = rand->bounded(activeTeams.size());

    return activeTeams[randomIndex];
}

void Tournament::advanceTournamentRound(const bool won)
{
    playerParticipating = won;
    if (!playerParticipating)
    {
        // simulate rest of tournament
    }
    else
    {
        currentRound++;
    }
}