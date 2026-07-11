#ifndef TOURNAMENTTABHANDLER_H
#define TOURNAMENTTABHANDLER_H

#include <vector>

#include "model/knights/Knight.h"
#include "model/tournaments/Tournament.h"

class TournamentTabHandler
{
public:
    TournamentTabHandler(std::vector<Tournament> todaysTournaments);

    std::vector<Tournament> &getAvailableTournaments()
    {
        return m_todaysTournaments;
    }

    void setTodaysTournaments(std::vector<Tournament> todaysTournaments)
    {
        m_todaysTournaments = std::move(todaysTournaments);
    }

    void registerSquadForTournament(size_t index, const std::vector<Knight*> &squad)
    {
        if (index < m_todaysTournaments.size())
        {
            m_todaysTournaments[index].registerPlayerTeam(squad);
        }
    }

    void cancelRegistration(size_t index)
    {
        if (index < m_todaysTournaments.size())
        {
            m_todaysTournaments[index].clearTournament();
        }
    }

    int getNextRegisteredTournamentIndex(size_t startingIndex) const;

private:
    std::vector<Tournament> m_todaysTournaments;
};

#endif