// Tournaments/Tournament.h
#ifndef _Tournament
#define _Tournament

#pragma once
#include <string>
#include <vector>
#include <list>
#include "Knights/Knight.h"

class Tournament
{
public:
    Tournament(std::string newName, int newTeammates, int newRounds, int newReward, std::string newHosterName)
    : name(newName), teammates(newTeammates), rounds(newRounds), reward(newReward), hosterName(newHosterName)
    {}

    // Accessors for UI rendering
    std::string getName() const { return name; }
    int getRequiredTeammates() const { return teammates; }
    int getReward() const { return reward; }
    std::string getHosterName() const { return hosterName; }

    // Roster tracking
    std::vector<Knight>& getSelectedTeam() { return playerTeam; }
    void setSelectedTeam(std::vector<Knight>& newPlayerTeam) { playerTeam = newPlayerTeam; };
    bool hasTeamAssembled() const { return !playerTeam.empty(); }
    void clearTeam() { playerTeam.clear(); }

private:
    std::string name;
    int teammates; // How many knights can enter (e.g., 1v1, 2v2)
    int rounds;
    int reward;
    std::string hosterName;
    std::vector<Knight> playerTeam; // Player's chosen fighting squad
    std::list<std::vector<Knight>> participants;
};

#endif