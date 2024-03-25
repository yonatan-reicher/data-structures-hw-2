#include "../olympics24a2.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <set>

int main() {
    olympics_t olympics;

    assert(StatusType::INVALID_INPUT == olympics.add_team(0));
    assert(StatusType::INVALID_INPUT == olympics.add_team(-1));
    assert(StatusType::SUCCESS == olympics.add_team(1));
    assert(StatusType::SUCCESS == olympics.add_team(2));
    assert(StatusType::FAILURE == olympics.add_team(1));

    std::cout << "test1 passed" << std::endl;

    return 0;
}
