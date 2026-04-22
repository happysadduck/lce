#include <stdio.h>
#include "raylib.h"
#include "render.h"
#include "core_to_render.h"
#include "constants.h"

static inline float GetMapScale(void)
{
    float scaleX = GetScreenWidth() / (float)WIN_WID;
    float scaleY = GetScreenHeight() / (float)WIN_HEI;
    return (scaleX < scaleY) ? scaleX : scaleY;
}

static void draw_predict_trajectory_period(const ship_t *ship, int period_tick_cnt, Color color)
{
    float scale = GetMapScale();
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
            continue;
        DrawCircleV((Vector2){x, y}, 0.05, color);
    }
}

static Color color_select(const map_t *map, const ship_t *ship)
{
    const Color colors[] = COLORS;
    if (ship->flag_ship_idx >= 0)
        return colors[ship->flag_ship_idx];
    return DARKGRAY;
}

void render_god_view(const map_t *map)
{
    float scale = GetMapScale();
    DrawRectangle(-MAP_WID / 2, -MAP_HEI / 2, MAP_WID, MAP_HEI, BLACK);
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
        DrawCircleV((Vector2){x, y}, ship_render_radius, color);
        draw_predict_trajectory_period(curr, 20, color);
    }
}

void render_god_minimap(const map_t *map)
{
    float scale = GetMapScale();
    DrawRectangle(MINIMAP_POS_X, MINIMAP_POS_Y, MINIMAP_WID, MINIMAP_HEI, BLACK);
    DrawRectangleLines(MINIMAP_POS_X, MINIMAP_POS_Y, MINIMAP_WID, MINIMAP_HEI, GRAY);
}

void render_flip(const map_t *map, const Camera2D *map_camera)
{
    /*TODO: 照理来说, 整地图以map_camera变换之后再进行map_to_actual变换*/
    /*这应当导致整个显示区域被填满, 甚至特定的窗口大小可以看到更多信息? 我觉得这样设计没有问题*/

    Camera2D designed_to_actual;
    designed_to_actual.target = (Vector2){WIN_WID / 2.0f, WIN_HEI / 2.0f};
    designed_to_actual.offset = (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    designed_to_actual.zoom = GetMapScale();
    designed_to_actual.rotation = 0.0f;

    Camera2D map_to_actual;
    map_to_actual.target = (Vector2){0.0f, 0.0f};
    map_to_actual.offset = (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    float scale_x = WIN_WID / MAP_WID;
    float scale_y = WIN_HEI / MAP_HEI;
    map_to_actual.zoom = (scale_x < scale_y ? scale_x : scale_y) * GetMapScale();
    map_to_actual.rotation = 0.0f;

    ClearBackground(WHITE);

    BeginMode2D(map_to_actual);
    render_god_view(map);
    EndMode2D();

    BeginMode2D(designed_to_actual);
    render_god_minimap(map);
    EndMode2D();
}