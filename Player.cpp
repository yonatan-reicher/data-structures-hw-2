#include "Player.h"
#include "PowerAndId.h"

Player::Player()
    : m_strength(0), m_next(nullptr), m_prev(nullptr)
{
    // nothing.
}

Player::Player(int strength, int id)
    : m_id(id), m_strength(strength), m_next(nullptr), m_prev(nullptr)
{
    // nothing.
}

void Player::setId(int id)
{
    m_id = id;
}

int Player::id() const
{
    return m_id;
}

int Player::strength() const
{
    return m_strength;
}

PowerAndId Player::key() const
{
    return PowerAndId(m_strength, m_id);
}

Player* Player::next() const
{
    return m_next;
}

Player* Player::prev() const
{
    return m_prev;
}

void Player::setNext(Player* next)
{
    m_next = next;
    if (next != nullptr)
    {
        next->m_prev = this;
    }
}

void Player::setPrev(Player* prev)
{
    m_prev = prev;
    if (prev != nullptr)
    {
        prev->m_next = this;
    }
}

const Player* Player::getPreviest() const
{
    if (m_prev == nullptr)
    {
        return this;
    }

    return m_prev->getPreviest();
}

Player* Player::getPreviest()
{
    const Player* const_this = this;
    return const_cast<Player*>(const_this->getPreviest());
}
