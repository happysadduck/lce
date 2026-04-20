#include "core_to_render.h"
#include "constants.h"

void map_pos_to_screen(double *x, double *y)
{
    double scale_x = WIN_WID / MAP_WID;
    double scale_y = WIN_HEI / MAP_HEI;
    double scale = scale_x < scale_y ? scale_x : scale_y;
    double map_x = *x;
    double map_y = *y;
    *x = WIN_WID * 0.5 + map_x * scale;
    *y = WIN_HEI * 0.5 - map_y * scale;
}
