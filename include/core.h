#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "constants.h"

/*
地图渲染规则:
  Ship:
  上帝看得见所有船, 船如果gg了, 需要用三种浅色表示船的状态:
    其中:
    A: g了, 但是由于此船发出的光尚没有消散, 仍可观测(用can_be_observed存储)
    B: g了, 但是阵亡信息还没有发送给母舰(用dead_msg_is_sent存储)
    A且B, 仅A, 仅B是三种颜色. 没有又不A又B的情况, 此时的船必定被移除.
  玩家就不能看见.
    玩家总是可以与上帝视角一样看见己方船, 只要dead_msg_is_sent还是false, 那就看得见. 否则看不见.
    而且, 玩家看已经dead但是dead_msg_is_sent的船视作为正常船只, 而没有浅色效果.
*/

typedef struct
{
    double x;
    double y;
} Point;

typedef struct Ship
{
    Point p;
    Point v;
    Point a;
    struct Ship *king;
    bool is_dead;
    bool can_be_observed;
    bool dead_msg_is_sent;
} Ship;

typedef struct
{
    Ship ships[MAX_SHIP_CNT];
} Map;