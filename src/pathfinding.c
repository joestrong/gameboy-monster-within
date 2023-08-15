#pragma bank 0

#include "pathfinding.h"

// Stores tile_x and tile_y pairs
UBYTE waypoints[] = {
    4, 2,   // 0
    28, 2,  // 1
    28, 10, // 2
};

// Stores next waypoint in journey to target waypoint
UBYTE waypoint_map[] = {
    // Waypoint 0
    0,
    1, // To 1
    1, // To 2 via 1
    // Waypoint 1
    0,
    1,
    2,
    // Waypoint 2
    1,
    1,
    2
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