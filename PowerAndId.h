#ifndef POWERANDID_H
#define POWERANDID_H

#include "Team.h"

struct PowerAndId {
    int m_strength;
    int m_id;

    PowerAndId(int power, int id);
    PowerAndId(const Team& team);

    bool operator<(const PowerAndId& other) const;
    bool operator>(const PowerAndId& other) const;
    bool operator==(const PowerAndId& other) const;
    bool operator!=(const PowerAndId& other) const;
};

#endif
