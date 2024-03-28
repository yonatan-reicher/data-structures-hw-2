#ifndef PLAYER_H
#define PLAYER_H

class PowerAndId;

class Player {
    int m_id;
    int m_strength;
    Player* m_next;
    Player* m_prev;

public:
    Player();
    Player(int strength, int id);

    void setId(int id);
    int id() const;

    int strength() const;

    PowerAndId key() const;

    Player* next() const;

    Player* prev() const;

    // Also sets the next player's prev to this player.
    void setNext(Player* next);

    // Also sets the next player's prev to this player.
    void setPrev(Player* prev);

    // Returns the last player in the list (prev direction).
    const Player* getPreviest() const;

    // Returns the last player in the list (prev direction).
    Player* getPreviest();
};

#endif
