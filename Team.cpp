#include "Team.h"

Team::Team(int teamId) : m_teamId(teamId), m_players(), m_newest(nullptr)
{
    // nothing.
}

int Team::teamId() const
{
    return m_teamId;
}

void Team::addPlayer(int playerStrength)
{
    Player& p = m_players.insert(playerStrength, Player(playerStrength));
    
    p.setPrev(m_newest);
    m_newest = &p;
}

bool Team::removeNewestPlayer()
{
    if (m_newest == nullptr)
    {
        return false;
    }

    if (m_newest->prev() == nullptr)
    {
        m_players.remove(m_newest->strength());
        m_newest = nullptr;
        assert(m_players.size() == 0);
        return true;
    }

    m_newest = m_newest->prev();
    m_players.remove(m_newest->next()->strength());
    m_newest->setNext(nullptr);
    return true;
}

int Team::size() const
{
    assert(m_players.size() == 0 ? m_newest == nullptr : m_newest != nullptr);
    return m_players.size();
}

int Team::getStrength() const
{
    if (size() == 0) {
        // TODO: Is this correct? Maybe we should throw?
        return 0;
    }

    int medianStrength = m_players.middleKey();
    return medianStrength * size();
}
