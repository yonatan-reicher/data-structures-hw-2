#include "../olympics24a2.h"
#include <cassert>
#include <iostream>

int main() {
    olympics_t o;

	o.play_tournament(3, 4);
	o.add_team(2);
	o.add_team(4);
	o.add_team(5);
	o.add_player(2, 1);
	o.add_player(4, 1);
	o.add_player(4, 5);
	o.play_match(2, 4);
	o.play_tournament(5, 3);
	o.add_player(3, 5);
	assert(11 == o.get_highest_ranked_team().ans());
	o.num_wins_for_team(2);
	o.num_wins_for_team(1);
	o.add_team(5);
	o.remove_newest_player(4);
	o.remove_newest_player(2);

    std::cout << "test1 passed" << std::endl;

    return 0;
}
