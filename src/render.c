#include "raylib.h"
#include "render.h"
#include "constants.h"

static inline float GetMapScale(void)
{
    float scaleX = GetScreenWidth() / (float)WIN_WID;
    float scaleY = GetScreenHeight() / (float)WIN_HEI;
    return (scaleX < scaleY) ? scaleX : scaleY; // 等比缩放，确保全部可见
}

static inline int MapToScreenX(int x)
{
    float scale = GetMapScale();
    int offsetX = (GetScreenWidth() - (int)(WIN_WID * scale)) / 2;
    return (int)(x * scale) + offsetX;
}

static inline int MapToScreenY(int y)
{
    float scale = GetMapScale();
    int offsetY = (GetScreenHeight() - (int)(WIN_HEI * scale)) / 2;
    return (int)(y * scale) + offsetY;
}

void render_background()
{
    float scale = GetMapScale();
    DrawRectangle(MapToScreenX(0), MapToScreenY(0), (int)(WIN_WID * scale), (int)(WIN_HEI * scale), BLACK);
}

void render_minimap(const map_t *map)
{
    float scale = GetMapScale();
    const Color colors[] = {RED, YELLOW, BLUE, GREEN};
    DrawRectangle(MapToScreenX(1620), MapToScreenX(40), (int)(260 * scale), (int)(260 * scale), GRAY);
    for (int i = 0; i < map->ship_cnt; i++)
    {
        Color color = colors[(map->ships[i].flag_ship - map->ships) / sizeof(ship_t)];
        /*TODO: 将地图上点的位置等比例转到小地图大小上, 然后根据颜色画圆, 母舰应当有特殊标记*/
        /*同时完善地图显示*/
    }
}
