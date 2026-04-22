#ifndef CORE_TO_RENDER_H
#define CORE_TO_RENDER_H

int is_in_map(float x, float y);
void map_pos_to_screen(float *x, float *y);
float map_to_screen_scale(void);

#endif