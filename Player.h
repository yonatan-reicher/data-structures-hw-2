#ifndef PLAYER_H
#define PLAYER_H

struct Player {
    Player* m_next;
    Player* m_prev;

    Player()
        : m_next(nullptr),
        m_prev(nullptr)
    {}
};

#endif
