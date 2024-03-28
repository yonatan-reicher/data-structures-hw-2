#include "olympics24a2.h"
#include <limits>

olympics_t::olympics_t()
{
	// nothing.
}

olympics_t::~olympics_t()
{
    // nothing too!
}

void olympics_t::removeAndUpdateTeamFromPowerTree(PowerAndId key)
{
    if (!m_teamsByPower.contains(key)) return;

    int wins = m_teamsByPower.wins(key);
    Team& team = *m_teamsByPower.get(key);
    team.setTempWins(wins);
    m_teamsByPower.remove(key);
}

void olympics_t::insertAndUpdateTeamFromPowerTree(Team& team)
{
    if (team.size() == 0) return;

    PowerAndId key = PowerAndId(team.getPower(), team.id());
    m_teamsByPower.insert(key, &team);
    int index = m_teamsByPower.getIndex(key);
    m_teamsByPower.addWinsInRange(index, index, team.getTempWins());
}

StatusType olympics_t::add_team(int teamId)
{
    if (teamId <= 0) return StatusType::INVALID_INPUT;
    if (m_teams.contains(teamId)) return StatusType::FAILURE;

    m_teams.insert(teamId, std::unique_ptr<Team>(new Team(teamId)));
    return StatusType::SUCCESS;
}

StatusType olympics_t::remove_team(int teamId)
{
    if (teamId <= 0) return StatusType::INVALID_INPUT;
    if (!m_teams.contains(teamId)) return StatusType::FAILURE;

    // Tree
    PowerAndId key = PowerAndId(m_teams.get(teamId)->getPower(), teamId);
    removeAndUpdateTeamFromPowerTree(key);

    // Table! The return value is ignored so it's destructor is called and the
    // team is no more!
    m_teams.remove(teamId);
    return StatusType::SUCCESS;
}

StatusType olympics_t::add_player(int teamId, int playerStrength)
{
    if (teamId <= 0 || playerStrength <= 0) return StatusType::INVALID_INPUT;
    if (!m_teams.contains(teamId)) return StatusType::FAILURE;

    Team& team = *m_teams.get(teamId);
    PowerAndId oldKey = PowerAndId(team.getPower(), teamId);

    team.addPlayer(playerStrength);

    // Update the power tree! Wappaooh!
    removeAndUpdateTeamFromPowerTree(oldKey);
    insertAndUpdateTeamFromPowerTree(team);

    return StatusType::SUCCESS;
}

StatusType olympics_t::remove_newest_player(int teamId)
{
    if (teamId <= 0) return StatusType::INVALID_INPUT;
    if (!m_teams.contains(teamId)) return StatusType::FAILURE;

    Team& team = *m_teams.get(teamId);
    PowerAndId oldKey = PowerAndId(team.getPower(), teamId);

    if (team.size() == 0) return StatusType::FAILURE;

    bool removed = team.removeNewestPlayer();
    assert(removed);

    // Update the power tree.
    removeAndUpdateTeamFromPowerTree(oldKey);
    insertAndUpdateTeamFromPowerTree(team);

    return StatusType::SUCCESS;
}

output_t<int> olympics_t::play_match(int teamId1, int teamId2)
{
    if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2)
    {
        return StatusType::INVALID_INPUT;
    }
    if(!m_teams.contains(teamId1) || !m_teams.contains(teamId2)
        || 0 == m_teams.get(teamId1)->size() || 0 == m_teams.get(teamId2)->size())
    {
        return StatusType::FAILURE;
    }

    Team& team1 = *m_teams.get(teamId1);
    Team& team2 = *m_teams.get(teamId2);
    int winnerIndex;

    if(team1.getPower() == team2.getPower())
    {
        winnerIndex = teamId1 < teamId2
            ? m_teamsByPower.getIndex(PowerAndId(team1.getPower(), teamId1))
            : m_teamsByPower.getIndex(PowerAndId(team2.getPower(), teamId2));
    }
    else
    {
        winnerIndex = team1.getPower() > team2.getPower()
            ? m_teamsByPower.getIndex(PowerAndId(team1.getPower(), teamId1))
            : m_teamsByPower.getIndex(PowerAndId(team2.getPower(), teamId2));
    }

    m_teamsByPower.addWinsInRange(winnerIndex, winnerIndex, 1);

    return m_teamsByPower.getKeyByIndex(winnerIndex).m_id;
}

output_t<int> olympics_t::num_wins_for_team(int teamId)
{
    if(teamId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    if(!m_teams.contains(teamId))
    {
        return StatusType::FAILURE;
    }

    Team& team = *m_teams.get(teamId);
    if(0 == team.size())
    {
        return team.getTempWins();
    }

    return m_teamsByPower.wins(PowerAndId(team.getPower(), teamId));
}

output_t<int> olympics_t::get_highest_ranked_team()
{
    if (0 == m_teams.size())
    {
        return -1;
    }
    if (0 == m_teamsByPower.size())
    {
        return 0;
    }
    return m_teamsByPower.getMaxRank();
}

StatusType olympics_t::unite_teams(int teamId1, int teamId2)
{
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) return StatusType::INVALID_INPUT;

    if (!m_teams.contains(teamId1) || !m_teams.contains(teamId2)) return StatusType::FAILURE;

    Team& team1 = *m_teams.get(teamId1);
    Team& team2 = *m_teams.get(teamId2);

    PowerAndId oldTeam1Key(team1.getPower(), teamId1);
    PowerAndId oldTeam2Key(team2.getPower(), teamId2);

    team1.mergeAndEat(team2);

    // Remove Team 2.
    removeAndUpdateTeamFromPowerTree(oldTeam2Key);
    m_teams.remove(teamId2);

    // Update the power tree!
    removeAndUpdateTeamFromPowerTree(oldTeam1Key);
    insertAndUpdateTeamFromPowerTree(team1);

    return StatusType::SUCCESS;
}


output_t<int> olympics_t::play_tournament(int lowPower, int highPower)
{
    if (lowPower <= 0 || highPower <= 0 || lowPower > highPower) return StatusType::INVALID_INPUT;

    PowerAndId lowKey = PowerAndId(lowPower, std::numeric_limits<int>::max());
    PowerAndId highKey = PowerAndId(highPower, std::numeric_limits<int>::min());
    int low = m_teamsByPower.getIndexOfLargerOrEqual(lowKey);
    int high = m_teamsByPower.getIndexOfSmallerOrEqual(highKey);

    int i = high - low + 1;

    if (i < 1) return StatusType::FAILURE;
    if (1 == i)
    {
        return m_teamsByPower.getKeyByIndex(high).m_id;
    }

    // Is the number of teams a power of 2?
    if ((i & (i - 1)) != 0) return StatusType::FAILURE;

    while (i > 1) {
        m_teamsByPower.addWinsInRange(high - i / 2, high, 1);
        i = i / 2;
    }

    return m_teamsByPower.getKeyByIndex(high).m_id;
}
