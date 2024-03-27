#include "../olympics24a2.h"
#include <cassert>
#include <iostream>

int main() {
    olympics_t o;

	o.add_team(4);
	o.add_player(4, 4);
	o.add_player(2, 4);
	o.add_team(2);
	o.add_team(5);
	o.add_player(5, 5);
	o.play_tournament(1, 2);
	o.add_team(5);
	o.add_team(5);
	o.add_player(5, 3);
	o.play_match(4, 5);

    std::cout << "test1 passed" << std::endl;

    return 0;
}
