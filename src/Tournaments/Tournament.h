#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#pragma once
#include <string>
#include <vector>
#include <cmath>
#include "Knights/Knight.h"

struct MatchUp {
    std::vector<Knight> teamA;
    std::vector<Knight> teamB;
    bool involvesPlayer = false;
};

class Tournament
{
public:
    Tournament(std::string newName, int newTeammates, int newRounds, int newReward, std::string newHosterName)
    : name(newName), teammates(newTeammates), maxRounds(newRounds), reward(newReward), hosterName(newHosterName),
      currentRound(1), playerParticipating(false)
    {}

    // Accessors
    std::string getName() const { return name; }
    int getRequiredTeammates() const { return teammates; }
    int getMaxRounds() const { return maxRounds; }
    int getCurrentRound() const { return currentRound; }
    bool isPlayerParticipating() const { return playerParticipating; }
    int getReward() const { return reward; }
    std::string getHost() const { return hosterName; }

    // Roster configuration
    void registerPlayerTeam(const std::vector<Knight>& team) { 
        playerTeam = team; 
        initializeTournament();
    }
    bool hasTeamAssembled() const { return !playerTeam.empty(); }
    void clearTournament() {
        playerTeam.clear();
        activeTeams.clear();
        currentRound = 1;
        playerParticipating = false;
    }

    // Bracket Match Engine Functions
    std::vector<MatchUp> generateCurrentRoundMatches();
    void advanceTournamentRound(const std::vector<std::vector<Knight>>& winningTeams);

private:
    void initializeTournament();

    std::string name;
    int teammates; 
    int maxRounds; // Total bracket levels (e.g., 3 rounds = Quarter, Semi, Finals)
    int reward;
    std::string hosterName;
    
    int currentRound; // Tracks 1, 2, or 3
    bool playerParticipating;

    std::vector<Knight> playerTeam;
    std::vector<std::vector<Knight>> activeTeams; // All remaining fighting squads left in brackets
};

#endif