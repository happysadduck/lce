#include <stdio.h>
#include "raylib.h"
#include "constants.h"
#include "core.h"
#include "render.h"

int main()
{
    InitWindow(WIN_WID, WIN_HEI, "game");
    map_t map;
    int current_tick = 0;
    prepare_map(&map, 4);
    map.ships[0].vx = -1;
    map.ships[0].ay = 0.1;
    map.ships[1].vy = 0.5;
    map.ships[1].ay = -0.02;
    map.ships[2].ax = 0.1;
    map.ships[2].ay = -0.2;
    map.ships[2].vy = 0.2;
    map.ships[3].ax = 0.2;
    map.ships[3].ay = 0.1;
    map.ships[3].vy = 0.5;
    while (!WindowShouldClose())
    {
        printf("tick: %d\n", current_tick++);
        double start = GetTime();
        BeginDrawing();
        ClearBackground(BLACK);
        render_background();
        render_god_view(&map);
        EndDrawing();

        update_map(&map);

        double elapsed = GetTime() - start;
        if (elapsed >= TICK_STEP)
        {
            printf("cale...\n");
            return -1;
        }
        WaitTime(TICK_STEP - elapsed);
    }
    CloseWindow();

    return 0;
}