// Tournaments/TournamentManager.h
#pragma once
#include <vector>
#include "Tournament.h"

class TournamentManager {
public:
    static TournamentManager& getInstance() {
        static TournamentManager instance;
        return instance;
    }

    std::vector<Tournament>& getActiveTournaments() { return activeTournaments; }

private:
    TournamentManager() {
        // Mock up two active tournaments for the board
        activeTournaments.emplace_back("The King's Grand Tournament", 2, 3, 500, "King Arthur");
        activeTournaments.emplace_back("Squire's Melee Brawl", 1, 2, 120, "Lord Cenric");
    }
    
    std::vector<Tournament> activeTournaments;
};