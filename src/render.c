#include <stdio.h>
#include "raylib.h"
#include "render.h"
#include "core_to_render.h"
#include "constants.h"

static inline float GetMapScale(void)
{
    float scaleX = GetScreenWidth() / (float)WIN_WID;
    float scaleY = GetScreenHeight() / (float)WIN_HEI;
    return (scaleX < scaleY) ? scaleX : scaleY; // 等比缩放，确保全部可见
}

static inline int MapToScreenX(int x, float scale)
{
    int offsetX = (GetScreenWidth() - (int)(WIN_WID * scale)) / 2;
    return (int)(x * scale) + offsetX;
}

static inline int MapToScreenY(int y, float scale)
{
    int offsetY = (GetScreenHeight() - (int)(WIN_HEI * scale)) / 2;
    return (int)(y * scale) + offsetY;
}

static void draw_predict_trajectory_period(const ship_t *ship, int period_tick_cnt, Color color)
{
    float scale = GetMapScale();
    float prev_x = ship->px;
    float prev_y = ship->py;
    float preiod_time = (float)period_tick_cnt * TICK_STEP;
    bool is_out = false;
    for (int i = 0; i < TRAJECTORY_SECTIONS; i++)
    {
        float t = preiod_time * i / TRAJECTORY_SECTIONS;
        float x = ship->px + ship->vx * t + 0.5f * ship->ax * t * t;
        float y = ship->py + ship->vy * t + 0.5f * ship->ay * t * t;
        if (!is_in_map(x, y))
            is_out = true;
        if (is_out)
        {
            x = prev_x;
            y = prev_y;
        }
        float trans_x = x;
        float trans_y = y;
        float trans_prev_x = prev_x;
        float trans_prev_y = prev_y;
        map_pos_to_screen(&trans_x, &trans_y);
        map_pos_to_screen(&trans_prev_x, &trans_prev_y);
        DrawLine(MapToScreenX(trans_x, scale), MapToScreenY(trans_y, scale),
                 MapToScreenX(trans_prev_x, scale), MapToScreenY(trans_prev_y, scale), color);
        prev_x = x;
        prev_y = y;
    }
}

static Color color_select(const map_t *map, const ship_t *ship)
{
    const Color colors[] = COLORS;
    if (ship->flag_ship_idx >= 0)
        return colors[ship->flag_ship_idx];
    return DARKGRAY;
}

void render_background()
{
    float scale = GetMapScale();
    DrawRectangle(MapToScreenX(0, scale), MapToScreenY(0, scale), (int)(WIN_WID * scale), (int)(WIN_HEI * scale), BLACK);
}

void render_god_view(const map_t *map)
{
    float scale = GetMapScale();
    for (int i = 0; i < map->ship_cnt; i++)
    {
        const ship_t *curr;
        curr = map->ships + i;
        Color color = color_select(map, curr);
        float x = curr->px;
        float y = curr->py;
        if (!is_in_map(x, y))
            continue;
        float ship_render_radius = SHIP_RADIUS;
        if (map->ships + curr->flag_ship_idx == curr)
            ship_render_radius *= 1.5;
        map_pos_to_screen(&x, &y);
        DrawCircle(MapToScreenX((int)x, scale), MapToScreenY((int)y, scale), ship_render_radius * scale, color);
        draw_predict_trajectory_period(curr, 20, color);
    }
}

void render_minimap(const map_t *map)
{
    float scale = GetMapScale();
    const Color colors[] = COLORS;
    DrawRectangle(MapToScreenX(1620, scale), MapToScreenX(40, scale), (int)(260 * scale), (int)(260 * scale), GRAY);
    for (int i = 0; i < map->ship_cnt; i++)
    {
        Color color = colors[map->ships[i].flag_ship_idx / sizeof(ship_t)];
        /*TODO: 将地图上点的位置等比例转到小地图大小上, 然后根据颜色画圆, 母舰应当有特殊标记*/
    }
}
