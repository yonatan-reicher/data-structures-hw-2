#include "Team.h"
#include "Player.h"

Team::Team(int teamId) : m_id(teamId), m_players(), m_newest(nullptr)
{
    // nothing.
}

int Team::id() const
{
    return m_id;
}

void Team::addPlayer(int playerStrength)
{
    std::unique_ptr<Player> player(new Player(playerStrength));
    Player& p = *player;
    m_players.insert(playerStrength, std::move(player));
    
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

int Team::getPower() const
{
    if (size() == 0) {
        // TODO: Is this correct? Maybe we should throw?
        return 0;
    }

    int medianStrength = m_players.middleKey();
    return medianStrength * size();
}

template <>
int getPower(const std::unique_ptr<Team>& data)
{
    return data->getPower();
}
