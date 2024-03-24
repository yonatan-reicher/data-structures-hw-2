#include "Player.h"

Player::Player(int strength)
    : m_strength(strength), m_next(nullptr), m_prev(nullptr)
{
    // nothing.
}

int Player::strength() const
{
    return m_strength;
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
