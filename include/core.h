#ifndef CORE_H
#define CORE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "constants.h"

/*
地图渲染规则:
  ship_t:
  上帝看得见所有船, 船如果gg了, 需要用三种浅色表示船的状态:
    其中:
    A: g了, 但是由于此船发出的光尚没有消散, 仍可观测(用can_be_observed存储)
    B: g了, 但是阵亡信息还没有发送给母舰(用dead_msg_is_sent存储)
    A且B, 仅A, 仅B是三种颜色. 没有又不A又B的情况, 此时的船必定被移除.
  玩家就不能看见.
    玩家总是可以与上帝视角一样看见己方船, 只要dead_msg_is_sent还是false, 那就看得见. 否则看不见.
    而且, 玩家看已经dead但是dead_msg_is_sent的船视作为正常船只, 而没有浅色效果.
  spot_t:
  上帝可以看见所有spot_t, 并且清晰知道它们的全部信息.
  玩家只能看见observer是友军的spot_t, 并且无法区分target
  hinder_t:
  玩家和上帝都可以看见所有hinder_t
  damage_src_t:
  上帝均可看见并可区分类别(激光/碎片云), 玩家无法看见.
*/

typedef struct
{
  double px;
  double py;
  double vx;
  double vy;
  double ax;
  double ay;
  int8_t flag_ship_idx;
  bool is_dead;
  bool can_be_observed;
  bool dead_msg_is_sent;
} ship_t;

typedef struct
{

} hinder_t;

typedef struct
{

} damage_src_t;

typedef struct
{
  ship_t ships[MAX_SHIP_CNT];
  uint16_t ship_cnt;
} map_t;

void prepare_map(map_t *map, int player_cnt);
void update_map(map_t *map);

#endif