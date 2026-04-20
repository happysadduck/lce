#include <stdlib.h>
#include "core.h"

void prepare_map(map_t *map, int player_cnt)
{
    map->ship_cnt = player_cnt;
    for (int i = 0; i < player_cnt; i++)
    {
        map->ships[i].flag_ship = map->ships + i;
        /*TODO: 目前母舰随机放置*/
        map->ships[i].px = (rand() % (int)(MAP_WID * 1000)) / 1000.0f - MAP_WID * 0.5f;
        map->ships[i].py = (rand() % (int)(MAP_HEI * 1000)) / 1000.0f - MAP_HEI * 0.5f;
    }
}