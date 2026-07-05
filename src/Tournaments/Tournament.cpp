// Tournaments/Tournament.cpp
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
            botTeam.push_back(Knight::generateRandomKnight());
        }
        activeTeams.push_back(botTeam);
    }
}

std::vector<MatchUp> Tournament::generateCurrentRoundMatches()
{
    std::vector<MatchUp> matchups;

    // Prevent out-of-bounds loops if activeTeams is empty or corrupted
    if (activeTeams.size() < 2)
        return matchups;

    // Ensure the loop stops safely before accessing i + 1
    for (size_t i = 0; i + 1 < activeTeams.size(); i += 2)
    {
        MatchUp match;
        match.teamA = activeTeams[i];
        match.teamB = activeTeams[i + 1];

        // 👍 FIX 3: Safe, defensive guard checks before accessing index [0]
        if (!match.teamA.empty() && !playerTeam.empty())
        {
            if (match.teamA[0].getName() == playerTeam[0].getName())
            {
                match.involvesPlayer = true;
            }
        }
        if (!match.teamB.empty() && !playerTeam.empty())
        {
            if (match.teamB[0].getName() == playerTeam[0].getName())
            {
                match.involvesPlayer = true;
            }
        }
        matchups.push_back(match);
    }
    return matchups;
}

void Tournament::advanceTournamentRound(const std::vector<std::vector<Knight>> &winningTeams)
{
    activeTeams = winningTeams; // Prune out losers entirely
    currentRound++;

    if (currentRound > maxRounds || activeTeams.size() <= 1)
    {
        playerParticipating = false; // Tournament concludes!
    }
}