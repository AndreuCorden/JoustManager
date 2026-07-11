#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#pragma once
#include <string>
#include <vector>
#include <cmath>
#include "model/knights/Knight.h"

class Tournament
{
public:
    Tournament(std::string newName, int newTeammates, int newRounds, int newReward, std::string newHosterName, int expPerRound)
    : m_name(newName)
    , m_teammates(newTeammates)
    , m_maxRounds(newRounds)
    , m_reward(newReward)
    , m_hosterName(newHosterName)
    , m_currentRound(1)
    , m_playerParticipating(false)
    , m_expPerRound(expPerRound)
    {}

    // Accessors
    std::string getName() const { return m_name; }
    int getRequiredTeammates() const { return m_teammates; }
    int getMaxRounds() const { return m_maxRounds; }
    int getCurrentRound() const { return m_currentRound; }
    bool isPlayerParticipating() const { return m_playerParticipating; }
    int getReward() const { return m_reward; }
    std::string getHost() const { return m_hosterName; }
    int getExpPerRound() const { return m_expPerRound; }
    std::vector<Knight*> getPlayerTeam() const { return m_playerTeam; }
    std::vector<Knight> getEnemyTeam() const;

    // Roster configuration
    void registerPlayerTeam(const std::vector<Knight*>& team) { 
        m_playerTeam = team; 
        initializeTournament();
    }
    bool hasTeamAssembled() const { return !m_playerTeam.empty(); }
    void clearTournament() {
        m_playerTeam.clear();
        m_activeTeams.clear();
        m_currentRound = 1;
        m_playerParticipating = false;
    }

    // Bracket Match Engine Functions
    void advanceTournamentRound(const bool won);

    void endTournament();

private:
    void initializeTournament();

    std::string m_name;
    int m_teammates; 
    int m_maxRounds; // Total bracket levels (e.g., 3 rounds = Quarter, Semi, Finals)
    int m_reward;
    std::string m_hosterName;
    int m_expPerRound;
    
    int m_currentRound; // Tracks 1, 2, or 3
    bool m_playerParticipating;

    std::vector<Knight*> m_playerTeam;
    std::vector<std::vector<Knight>> m_activeTeams; // All remaining fighting squads left in brackets
};

#endif