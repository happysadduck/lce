#include <stdlib.h>
#include "core.h"
#include "core_to_render.h"

static void rm_ship(map_t *map, int ship_id)
{
    // if (map->ships[ship_id].flag_ship != map->ships + ship_id)
    // {
    //     map->ships[ship_id] = map->ships[map->ship_cnt--];
    //     return;
    // }
    for (int i = 0; i < map->ship_cnt; i++)
    {
        if (map->ships[i].flag_ship == map->ships + ship_id)
            map->ships[i].flag_ship = NULL;
    }
}

void prepare_map(map_t *map, int player_cnt)
{
    map->ship_cnt = player_cnt;
    for (int i = 0; i < player_cnt; i++)
    {
        ship_t *curr = map->ships + i;
        curr->flag_ship = map->ships + i;
        /*TODO: 目前母舰随机放置*/
        curr->px = (rand() % (int)(MAP_WID * 1000)) / 1000.0f - MAP_WID * 0.5f;
        curr->py = (rand() % (int)(MAP_HEI * 1000)) / 1000.0f - MAP_HEI * 0.5f;
        curr->vx = 0;
        curr->vy = 0;
        curr->ax = 0;
        curr->ay = 0;
    }
}

void update_map(map_t *map)
{
    for (int i = 0; i < map->ship_cnt; i++)
    {
        ship_t *curr = map->ships + i;
        curr->px += 0.5f * curr->ax * TICK_STEP * TICK_STEP + curr->vx * TICK_STEP;
        curr->py += 0.5f * curr->ay * TICK_STEP * TICK_STEP + curr->vy * TICK_STEP;
        curr->vx += curr->ax * TICK_STEP;
        curr->vy += curr->ay * TICK_STEP;
        if (!is_in_map(curr->px, curr->py))
            rm_ship(map, curr - map->ships);
    }
}
