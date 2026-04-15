#include "core.h"

void prepare_map(map_t *map, int player_cnt)
{
    for (int i = 0; i < player_cnt; i++)
    {
        map->ships[i].flag_ship = map->ships + i;
    }
}