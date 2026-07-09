#include "model/tournaments/TournamentTabHandler.h"

TournamentTabHandler::TournamentTabHandler(std::vector<Tournament> todaysTournaments)
: m_todaysTournaments(todaysTournaments)
{
}

int TournamentTabHandler::getNextRegisteredTournamentIndex(size_t startingIndex) const
{
    // Find the next tournament where the player actually signed up
    for (size_t i = startingIndex; i < m_todaysTournaments.size(); ++i)
    {
        if (m_todaysTournaments[i].isPlayerParticipating())
        {
            return static_cast<int>(i); // Found one!
        }
    }
    return -1; // No more registered tournaments left for today
}