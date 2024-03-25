#ifndef TEAM_H
#define TEAM_H

#include "tree.h"
#include "Player.h"

class Team {
    int m_teamId;
    Tree<int, Player> m_players; 
    Player* m_newest;

public:
    Team(int teamId);

    int teamId() const;

    void addPlayer(int playerStrength);

    int size() const;
    
    bool removeNewestPlayer();

    int getPower() const;
};

#endif
