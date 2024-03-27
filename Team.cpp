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

using P = std::unique_ptr<Player>;

void combine(P* array1[], int size1, P* array2[], int size2, P dest[]) {
    int i = 0;
    int j = 0;
    while (i < size1 && j < size2) {
        P& p1 = *array1[i];
        P& p2 = *array2[j];
        if (p1->strength() < p2->strength()) {
            dest[i + j] = std::move(p1);
            i++;
        } else {
            dest[i + j] = std::move(p2);
            j++;
        }
    }

    while (i < size1) {
        P& p1 = *array1[i];
        dest[i + j] = std::move(p1);
        i++;
    }

    while (j < size2) {
        P& p2 = *array2[j];
        dest[i + j] = std::move(p2);
        j++;
    }
}

int playerStrength(P& p) {
    return p->strength();
}

int Team::mergeAndEat(Team& other) {
    std::unique_ptr<P*[]> myPlayers(m_players.toArray());
    std::unique_ptr<P*[]> hisPlayers(other.m_players.toArray());
    int mySize = m_players.size();
    int hisSize = other.m_players.size();
    std::unique_ptr<P[]> newPlayers(new P[mySize + hisSize]);

    // First deal with the linked list.
    if (other.m_newest == nullptr) {
        // Nothing!
    } else if (m_newest == nullptr) {
        m_newest = other.m_newest;
    } else {
        Player* hisOldest = other.m_newest->getPreviest();
        m_newest->setNext(hisOldest);
        m_newest = other.m_newest;
    }
    
    // TODO: Is this correct? Also, we should implement combine and check that everything is deleted correctly.
    combine(
        myPlayers.get(), mySize,
        hisPlayers.get(), hisSize,
        newPlayers.get()
    );

    Tree<int, P> newTree = Tree<int, P>::fromArray(
        newPlayers.get(), mySize + hisSize, playerStrength
    );

    m_players = std::move(newTree);
}
