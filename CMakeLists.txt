cmake_minimum_required(VERSION 3.23)
project(data_structures_hw_2)

set(CMAKE_CXX_STANDARD 14)

add_executable(data_structures_hw_2 main24a2.cpp hashtable.h node.h
        olympics24a2.cpp olympics24a2.h Player.cpp Player.h PowerAndId.cpp
        PowerAndId.h Team.cpp Team.h tree.h wet2util.h)

add_executable(test1 Tests/test1.cpp hashtable.h node.h
        olympics24a2.cpp olympics24a2.h Player.cpp Player.h PowerAndId.cpp
        PowerAndId.h Team.cpp Team.h tree.h wet2util.h Tests/get_highest_ranked_team.cpp)

add_executable(test-play_match Tests/get_highest_ranked_team.cpp hashtable.h node.h
        olympics24a2.cpp olympics24a2.h Player.cpp Player.h PowerAndId.cpp
        PowerAndId.h Team.cpp Team.h tree.h wet2util.h)

add_executable(test-num_wins_for_team Tests/test-num_wins_for_team.cpp hashtable.h node.h
        olympics24a2.cpp olympics24a2.h Player.cpp Player.h PowerAndId.cpp
        PowerAndId.h Team.cpp Team.h tree.h wet2util.h)

add_executable(test-get_highest_ranked_team Tests/test-get_highest_ranked_team.cpp hashtable.h node.h
        olympics24a2.cpp olympics24a2.h Player.cpp Player.h PowerAndId.cpp
        PowerAndId.h Team.cpp Team.h tree.h wet2util.h)
