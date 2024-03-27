#ifndef TEAM_H
#define TEAM_H

#include "tree.h"
#include "Player.h"
#include "PowerAndId.h"
class PowerAndId;
class Player;

class Team {
    int m_id;
    Tree<PowerAndId, std::unique_ptr<Player>> m_players; 
    Player* m_newest;
    int m_nextId;
    int m_tempWins;

public:
    Team(int teamId);

    int id() const;

    void addPlayer(int playerStrength);

    int size() const;
    
    bool removeNewestPlayer();

    int getPower() const;

    void mergeAndEat(Team& other);

    int getTempWins() const;

    void setTempWins(int m_tempWins);
};

// Specialization for Team so only it uses maxRank!
template <>
int getPower(Team* const& data);

#endif
