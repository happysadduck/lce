#ifndef RENDER_H
#define RENDER_H

#include "core.h"

/*raylib可能是世界上最好的C99图形库, 当窗口建立之后, 一切操作均视为在该窗口上画画, 实在是过于方便*/
void render_background();
void render_god_view(const map_t *map);
void render_minimap(const map_t *map);
void render_major_map(const map_t *map);

#endif