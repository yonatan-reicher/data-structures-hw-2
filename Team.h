#ifndef TEAM_H
#define TEAM_H

#include "tree.h"
#include "Player.h"

class Team {
    int m_id;
    Tree<int, std::unique_ptr<Player>> m_players; 
    Player* m_newest;

public:
    Team(int teamId);

    int id() const;

    void addPlayer(int playerStrength);

    int size() const;
    
    bool removeNewestPlayer();

    int getPower() const;

    int mergeAndEat(Team& other);
};

// Specialization for Team so only it uses maxRank!
template <>
int getPower(const std::unique_ptr<Team>& data);

#endif
