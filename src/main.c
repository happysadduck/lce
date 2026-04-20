#include <stdio.h>
#include "raylib.h"
#include "constants.h"
#include "core.h"
#include "render.h"

int main()
{
    InitWindow(WIN_WID, WIN_HEI, "game");
    map_t map;
    prepare_map(&map, 4);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        render_background();
        render_god_view(&map);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}