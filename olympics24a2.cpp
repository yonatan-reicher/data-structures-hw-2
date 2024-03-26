#include "olympics24a2.h"

olympics_t::olympics_t()
{
	// nothing.
}

olympics_t::~olympics_t()
{
    // nothing too!
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
    if (m_teamsByPower.contains(key)) {
        m_teamsByPower.remove(key);
    }

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
    PowerAndId newKey = PowerAndId(team.getPower(), teamId);

    // Update the power tree! Wappaooh!
    if (m_teamsByPower.contains(oldKey)) m_teamsByPower.remove(oldKey);
    m_teamsByPower.insert(newKey, &team);

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
    PowerAndId newKey = PowerAndId(team.getPower(), teamId);

    // Update the power tree.
    if (m_teamsByPower.contains(oldKey)) m_teamsByPower.remove(oldKey);
    if (m_teamsByPower.size() > 0) m_teamsByPower.insert(newKey, &team);

    return StatusType::SUCCESS;
}

output_t<int> olympics_t::play_match(int teamId1, int teamId2)
{
    // TODO: Your code goes here
    return 2008;
}

output_t<int> olympics_t::num_wins_for_team(int teamId)
{
    // TODO: Your code goes here
    static int i = 0;
    return (i++==0) ? 11 : 2;
}

output_t<int> olympics_t::get_highest_ranked_team()
{
	// TODO: Your code goes here
    return 42;
}

StatusType olympics_t::unite_teams(int teamId1, int teamId2)
{
	// TODO: Your code goes here
    return StatusType::SUCCESS;
}

output_t<int> olympics_t::play_tournament(int lowPower, int highPower)
{
    // TODO: Your code goes here
    // Why is there a static variable in my code?
    static int i = 0;
    return (i++==0) ? 11 : 2;
}
