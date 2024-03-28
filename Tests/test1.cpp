#include "../olympics24a2.h"
#include <cassert>
#include <iostream>

int main() {
    olympics_t o;

	o.play_match(3, 3);
	o.add_team(5);
	o.add_team(3);
	o.add_player(3, 4);
	o.get_highest_ranked_team();
	o.add_team(4);
	o.add_player(2, 5);
	o.remove_newest_player(2);
	o.num_wins_for_team(5);
	o.unite_teams(4, 3);
	o.add_player(4, 4);
	assert(StatusType::FAILURE == o.num_wins_for_team(3).status());

    std::cout << "test1 passed" << std::endl;

    return 0;
}
