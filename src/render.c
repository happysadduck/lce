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

static void draw_predict_trajectory_period(const ship_t *ship, int period_tick_cnt, Color color)
{
    double prev_x = ship->px;
    double prev_y = ship->py;
    double preiod_time = (double)period_tick_cnt * TICK_STEP;
    for (int i = 0; i < TRAJECTORY_SECTIONS; i++)
    {
        double t = preiod_time * i / TRAJECTORY_SECTIONS;
        double x = ship->px + ship->vx * t + 0.5f * ship->ax * t * t;
        double y = ship->py + ship->vy * t + 0.5f * ship->ay * t * t;
        double trans_x = x;
        double trans_y = y;
        double trans_prev_x = prev_x;
        double trans_prev_y = prev_y;
        map_pos_to_screen(&trans_x, &trans_y);
        map_pos_to_screen(&trans_prev_x, &trans_prev_y);
        DrawLine(MapToScreenX(trans_x), MapToScreenY(trans_y),
                 MapToScreenX(trans_prev_x), MapToScreenY(trans_prev_y), color);
        prev_x = x;
        prev_y = y;
    }
}

void render_background()
{
    float scale = GetMapScale();
    DrawRectangle(MapToScreenX(0), MapToScreenY(0), (int)(WIN_WID * scale), (int)(WIN_HEI * scale), BLACK);
}

void render_god_view(const map_t *map)
{
    float scale = GetMapScale();
    const Color colors[] = COLORS;
    for (int i = 0; i < map->ship_cnt; i++)
    {
        const ship_t *curr;
        curr = map->ships + i;
        Color color = colors[curr->flag_ship - map->ships];
        double x = curr->px;
        double y = curr->py;
        double ship_render_radius = SHIP_RADIUS;
        if (curr->flag_ship == curr)
            ship_render_radius *= 1.5;
        map_pos_to_screen(&x, &y);
        DrawCircle(MapToScreenX((int)x), MapToScreenY((int)y), ship_render_radius * scale, color);
        draw_predict_trajectory_period(curr, 5000, color);
    }
}

void render_minimap(const map_t *map)
{
    float scale = GetMapScale();
    const Color colors[] = COLORS;
    DrawRectangle(MapToScreenX(1620), MapToScreenX(40), (int)(260 * scale), (int)(260 * scale), GRAY);
    for (int i = 0; i < map->ship_cnt; i++)
    {
        Color color = colors[(map->ships[i].flag_ship - map->ships) / sizeof(ship_t)];
        /*TODO: 将地图上点的位置等比例转到小地图大小上, 然后根据颜色画圆, 母舰应当有特殊标记*/
    }
}
