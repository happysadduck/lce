#ifndef RENDER_H
#define RENDER_H

#include "core.h"

void render_god_view(const map_t *map);
void render_minimap(const map_t *map);
void render_major_map(const map_t *map);
void render_god_minimap(const map_t *map);
void render_flip(const map_t *map, const Camera2D *map_camera);

#endif