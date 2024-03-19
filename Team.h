#ifndef TEAM_H
#define TEAM_H

#include "tree.h"
#include "Player.h"

class Team {
    Tree<int, Player> m_players; 
};

#endif
