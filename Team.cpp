#include "Team.h"
#include "Player.h"
#include "PowerAndId.h"

Team::Team(int teamId)
    : m_id(teamId), m_players(), m_newest(nullptr)
    , m_nextId(1), m_tempWins(0)
{
    // nothing.
}

int Team::id() const
{
    return m_id;
}

void Team::addPlayer(int playerStrength)
{
    int id = m_nextId++;
    PowerAndId key(playerStrength, id);
    std::unique_ptr<Player> player(new Player(playerStrength, id));
    Player& p = *player;
    m_players.insert(key, std::move(player));
    
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
        m_players.remove(m_newest->key());
        m_newest = nullptr;
        assert(m_players.size() == 0);
        return true;
    }

    m_newest = m_newest->prev();
    m_players.remove(m_newest->next()->key());
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

    int medianStrength = m_players.middleKey().m_power;
    return medianStrength * size();
}

template <>
int getPower(Team* const& data)
{
    if (data == nullptr) {
        return 0;
    }
    return data->getPower();
}

using P = std::unique_ptr<Player>;

void addToId(P array[], int size, int add) {
    for (int i = 0; i < size; i++) {
        P& p = array[i];
        p->setId(p->id() + add);
    }
}

void combine(P array1[], int size1, P array2[], int size2, P dest[]) {
    int i = 0;
    int j = 0;
    while (i < size1 && j < size2) {
        P& p1 = array1[i];
        P& p2 = array2[j];
        if (p1->key() < p2->key()) {
            dest[i + j] = std::move(p1);
            i++;
        } else {
            dest[i + j] = std::move(p2);
            j++;
        }
    }

    while (i < size1) {
        P& p1 = array1[i];
        dest[i + j] = std::move(p1);
        i++;
    }

    while (j < size2) {
        P& p2 = array2[j];
        dest[i + j] = std::move(p2);
        j++;
    }
}

PowerAndId playerKey(P& p) {
    return p->key();
}

Player* nextNewest(Player* team1Newest, Player* team2Newest) {
    if (team1Newest == nullptr) return team2Newest;
    if (team2Newest == nullptr) return team1Newest;
    Player* team2Oldest = team2Newest->getPreviest();
    team1Newest->setNext(team2Oldest);
    return team2Newest;
}

void Team::mergeAndEat(Team& other) {
    int mySize = m_players.size();
    int hisSize = other.m_players.size();
    int hisNextId = other.m_nextId;

    // First deal with the linked list.
    // This must be done before getting the arrays, because when gettting them,
    // we remove all players and then other.m_newest must be nullptr.
    Player* newest1 = m_newest;
    Player* newest2 = other.m_newest;
    m_newest = nullptr;
    other.m_newest = nullptr;

    std::unique_ptr<P[]> myPlayers(m_players.toArray());
    std::unique_ptr<P[]> hisPlayers(other.m_players.toArray());
    std::unique_ptr<P[]> newPlayers(new P[mySize + hisSize]);
    
    addToId(hisPlayers.get(), hisSize, m_nextId);
    combine(
        myPlayers.get(), mySize,
        hisPlayers.get(), hisSize,
        newPlayers.get()
    );

    Tree<PowerAndId, P> newTree = Tree<PowerAndId, P>::fromArray(
        newPlayers.get(), mySize + hisSize, playerKey
    );

    m_players = std::move(newTree);
    m_nextId += hisNextId;
    // m_tempWins += other.m_tempWins;
    m_newest = nextNewest(newest1, newest2);
}

int Team::getTempWins() const {
    return m_tempWins;
}

void Team::setTempWins(int m_tempWins) {
    Team::m_tempWins = m_tempWins;
}
