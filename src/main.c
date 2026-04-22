#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "raylib.h"
#include "constants.h"
#include "core.h"
#include "render.h"

bool map_pointer_pos = 0;

void *core_calculate(void *arg)
{
    map_t *map_next_tick = arg;

    if (!map_pointer_pos)
    {
        map_next_tick++;
        map_pointer_pos = 1;
    }
    else
    {
        map_next_tick--;
        map_pointer_pos = 0;
    }

    memcpy(map_next_tick, arg, sizeof(map_t));
    update_map(map_next_tick);
    return map_next_tick;
}

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(WIN_WID, WIN_HEI, "game");
    SetTargetFPS((int)1 / RENDER_INTERVAL);

    pthread_t thread;

    map_t *map = (map_t *)malloc(2 * sizeof(map_t));
    prepare_map(map, 4);

    int flip = 0;

    map->ships[0].vx = -1;
    map->ships[0].ay = 0.1;
    map->ships[1].vy = 0.5;
    map->ships[1].ay = -0.02;
    map->ships[2].ax = 0.1;
    map->ships[2].ay = -0.2;
    map->ships[2].vy = 0.2;
    map->ships[3].ax = 0.2;
    map->ships[3].ay = 0.1;
    map->ships[3].vy = 0.5;

    pthread_create(&thread, NULL, &core_calculate, map);

    while (!WindowShouldClose())
    {
        float start = GetTime();
        BeginDrawing();

        render_flip(map, NULL);

        DrawFPS(0, 0);

        flip++;
        if (flip == RENDER_PER_TICK)
        {
            void *ret_ptr = map;
            pthread_join(thread, &ret_ptr);
            map = (map_t *)ret_ptr;
            pthread_create(&thread, NULL, core_calculate, map);
            flip = 0;
        }

        float elapsed = GetTime() - start;
        if (elapsed > RENDER_INTERVAL)
        {
            printf("cale...\n");
        }

        EndDrawing();
    }
    CloseWindow();

    return 0;
}