#include "../olympics24a2.h"

#define INVALID_TEAM (-1)
#define NOT_EXISTING_TEAM 69
#define EMPTY_TEAM 10
#define TEAM_STRONG 3
#define TEAM_WEAK1 1
#define TEAM_WEAK2 2

int main(){
    olympics_t olympics;

    olympics.add_team(EMPTY_TEAM);
    olympics.add_team(TEAM_WEAK1);
    olympics.add_team(TEAM_WEAK2);
    olympics.add_team(TEAM_STRONG);

    olympics.add_player(TEAM_STRONG, 1);
    olympics.add_player(TEAM_STRONG, 2);
    olympics.add_player(TEAM_STRONG, 3);
    olympics.add_player(TEAM_STRONG, 10);
    // middle = 3, size = 4

    olympics.add_player(TEAM_WEAK1, 1);
    olympics.add_player(TEAM_WEAK2, 1);

    assert(StatusType::INVALID_INPUT == olympics.play_match(INVALID_TEAM, TEAM_WEAK1).status());
    assert(StatusType::INVALID_INPUT == olympics.play_match(TEAM_WEAK1, INVALID_TEAM).status());

    assert(StatusType::FAILURE == olympics.play_match(EMPTY_TEAM, TEAM_WEAK1).status());
    assert(StatusType::FAILURE == olympics.play_match(TEAM_WEAK1, EMPTY_TEAM).status());

    assert(StatusType::FAILURE == olympics.play_match(NOT_EXISTING_TEAM, TEAM_WEAK1).status());
    assert(StatusType::FAILURE == olympics.play_match(TEAM_WEAK1, NOT_EXISTING_TEAM).status());

    assert(StatusType::INVALID_INPUT == olympics.play_match(TEAM_WEAK1, TEAM_WEAK1).status());

    assert(StatusType::SUCCESS == olympics.play_match(TEAM_WEAK1, TEAM_STRONG).status());
    assert(1 == olympics.num_wins_for_team(TEAM_STRONG).ans());

    assert(StatusType::SUCCESS == olympics.play_match(TEAM_WEAK1, TEAM_WEAK2).status());
    assert(1 == olympics.num_wins_for_team(TEAM_WEAK1).ans());

    return 0;
}