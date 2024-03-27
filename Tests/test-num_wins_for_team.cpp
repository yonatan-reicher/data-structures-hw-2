#include "../olympics24a2.h"

#define INVALID_TEAM (-1)
#define NOT_EXISTING_TEAM 69
#define EMPTY_TEAM 10
#define TEAM_STRONG 3
#define TEAM_WEAK 1

int main(){
    olympics_t olympics;

    olympics.add_team(EMPTY_TEAM);
    olympics.add_team(TEAM_WEAK);
    olympics.add_team(TEAM_STRONG);

    olympics.add_player(TEAM_STRONG, 1);
    olympics.add_player(TEAM_STRONG, 2);
    olympics.add_player(TEAM_STRONG, 3);
    olympics.add_player(TEAM_STRONG, 10);
    // middle = 3, size = 4

    olympics.add_player(TEAM_WEAK, 1);

    assert(StatusType::INVALID_INPUT == olympics.num_wins_for_team(INVALID_TEAM).status());

    assert(StatusType::FAILURE == olympics.num_wins_for_team(NOT_EXISTING_TEAM).status());

    olympics.play_match(TEAM_WEAK, TEAM_STRONG);
    assert(1 == olympics.num_wins_for_team(TEAM_STRONG).ans());

    assert(0 == olympics.num_wins_for_team(EMPTY_TEAM).ans());

    olympics.add_player(EMPTY_TEAM, 1000);
    olympics.play_match(TEAM_STRONG, EMPTY_TEAM);
    assert(1 == olympics.num_wins_for_team(EMPTY_TEAM).ans());
    olympics.remove_newest_player(EMPTY_TEAM);
    assert(1 == olympics.num_wins_for_team(EMPTY_TEAM).ans());

    return 0;
}