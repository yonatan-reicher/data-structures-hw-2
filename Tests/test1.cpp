#include "../olympics24a2.h"
#include <cassert>
#include <iostream>

int main() {
    olympics_t o;

	o.add_player(1, 0);
	o.add_team(2);
	o.num_wins_for_team(2);
	o.add_player(-1, 0);
	o.unite_teams(-1, 0);
	o.add_player(2, 2);
	o.play_tournament(1, 2);
	o.add_team(1);
	o.remove_team(2);

    std::cout << "test1 passed" << std::endl;

    return 0;
}
