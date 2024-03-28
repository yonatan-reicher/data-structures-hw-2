#ifndef POWERANDID_H
#define POWERANDID_H

#include "Team.h"
class Team;

struct PowerAndId {
    int m_power;
    int m_id;

    PowerAndId();
    PowerAndId(int power, int id);
    PowerAndId(const Team& team);

    bool operator<(const PowerAndId& other) const;
    bool operator>(const PowerAndId& other) const;
    bool operator==(const PowerAndId& other) const;
    bool operator!=(const PowerAndId& other) const;
};

#endif
