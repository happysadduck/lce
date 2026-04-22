#include "core_to_render.h"
#include "constants.h"

int is_in_map(float x, float y)
{
    return (x >= -(MAP_WID / 2) && x <= (MAP_WID / 2) && y >= -(MAP_HEI / 2) && y <= (MAP_HEI / 2));
}

float map_to_screen_scale()
{
    float scale_x = WIN_WID / MAP_WID;
    float scale_y = WIN_HEI / MAP_HEI;
    return scale_x < scale_y ? scale_x : scale_y;
}

void map_pos_to_screen(float *x, float *y)
{
    float scale_x = WIN_WID / MAP_WID;
    float scale_y = WIN_HEI / MAP_HEI;
    float scale = scale_x < scale_y ? scale_x : scale_y;
    float map_x = *x;
    float map_y = *y;
    *x = WIN_WID * 0.5 + map_x * scale;
    *y = WIN_HEI * 0.5 - map_y * scale;
}
