#include <stdio.h>
#include "raylib.h"
#include "constants.h"
#include "core.h"
#include "render.h"

int main()
{
    InitWindow(WIN_WID, WIN_HEI, "game");
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        render_background();
        map_t *map = NULL;
        render_minimap(map);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}