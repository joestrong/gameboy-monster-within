#pragma bank 255

#include "overworld.h"
#include "./globals.h"
#include "./tiles/overworld.h"

void load_overworld() BANKED {
  set_bkg_data(overworld_baseTile, overworld_TILE_COUNT, overworld_tiles);
  set_bkg_submap(camera_x >> 3, camera_y >> 3, 21, 19, overworld_map, bigmap_mapWidth);
}

void set_camera() BANKED {
    SCY_REG = camera_y;
    SCX_REG = camera_x; 
    
    map_pos_y = (uint8_t)(camera_y >> 3u);
    if (map_pos_y != old_map_pos_y) { 
        if (camera_y < old_camera_y) {
            set_bkg_submap(map_pos_x, map_pos_y, MIN(21u, bigmap_mapWidth-map_pos_x), 1, overworld_map, bigmap_mapWidth);
        } else {
            if ((bigmap_mapHeight - 18u) > map_pos_y) {
              set_bkg_submap(map_pos_x, map_pos_y + 18u, MIN(21u, bigmap_mapWidth-map_pos_x), 1, overworld_map, bigmap_mapWidth);
            }
        }
        old_map_pos_y = map_pos_y; 
    }
    
    map_pos_x = (uint8_t)(camera_x >> 3u);
    if (map_pos_x != old_map_pos_x) {
        if (camera_x < old_camera_x) {
            set_bkg_submap(map_pos_x, map_pos_y, 1, MIN(19u, bigmap_mapHeight - map_pos_y), overworld_map, bigmap_mapWidth);     
        } else {
            if ((bigmap_mapWidth - 20u) > map_pos_x) set_bkg_submap(map_pos_x + 20u, map_pos_y, 1, MIN(19u, bigmap_mapHeight - map_pos_y), overworld_map, bigmap_mapWidth);     
        }
        old_map_pos_x = map_pos_x;
    }
    
    old_camera_x = camera_x;
    old_camera_y = camera_y;
}