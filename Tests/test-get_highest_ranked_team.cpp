#include "../olympics24a2.h"

#define EMPTY_TEAM 10
#define TEAM_STRONG 3
#define TEAM_WEAK1 1
#define TEAM_WEAK2 2

int main(){
    olympics_t olympics;

    assert(-1 == olympics.get_highest_ranked_team().ans());

    olympics.add_team(EMPTY_TEAM);
    olympics.add_team(TEAM_WEAK1);
    olympics.add_team(TEAM_WEAK2);
    olympics.add_team(TEAM_STRONG);

    assert(0 == olympics.get_highest_ranked_team().ans());

//    olympics.add_player(TEAM_STRONG, 1);
//    olympics.add_player(TEAM_STRONG, 2);
//    olympics.add_player(TEAM_STRONG, 3);
//    olympics.add_player(TEAM_STRONG, 10);
//    // middle = 3, size = 4
//
//    olympics.add_player(TEAM_WEAK1, 1);
//    olympics.add_player(TEAM_WEAK2, 1);

    return 0;
}