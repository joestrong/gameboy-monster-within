#include <gb/gb.h>

#ifndef PATHFINDING_H
#define PATHFINDING_H

#define WAYPOINT_COUNT 6

struct waypoint {
    UBYTE id;
    UBYTE x;
    UBYTE y;
};
typedef struct waypoint waypoint;

waypoint get_waypoint(UBYTE id);

waypoint get_next_waypoint(UBYTE current, UBYTE target);

#endif