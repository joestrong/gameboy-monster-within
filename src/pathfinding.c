#pragma bank 0

#include "pathfinding.h"

// Stores tile_x and tile_y pairs
const UBYTE waypoints[] = {
    8, 2,   // 0
    37, 2,  // 1
    37, 10, // 2

    37, 18, // 3

    30, 20, // 4
    10, 20, // 5
};

// Stores next waypoint in journey to target waypoint
const UBYTE waypoint_map[] = {
    // Waypoint 0
    0,
    1, // To 1
    1, // To 2 via 1
    1,
    1,
    1,
    // Waypoint 1
    0,
    1,
    2,
    2,
    2,
    2,
    // Waypoint 2
    1,
    1,
    2,
    3,
    3,
    3,
    // Waypoint 3
    2,
    2,
    2,
    3,
    4,
    4,
    // Waypoint 4
    2,
    2,
    2,
    3,
    4,
    5,
    // Waypoint 5
    2,
    2,
    2,
    3,
    4,
    5,
};

waypoint get_waypoint(UBYTE id) {
    waypoint my_waypoint = {
        .id = id,
        .x = waypoints[id * 2],
        .y = waypoints[id * 2 + 1],
    };

    return my_waypoint;
}

waypoint get_next_waypoint(UBYTE current, UBYTE target) {
     waypoint my_waypoint;
     my_waypoint = get_waypoint(waypoint_map[current * WAYPOINT_COUNT + target]);

     return my_waypoint;
}