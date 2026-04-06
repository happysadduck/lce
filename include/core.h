#ifndef CORE_H
#define CORE_H

#include "config.h"
#include "pool.h"

/*TODO: 认为plan controller中的plan和monitored应该用哈希表比较好. 或需要id分配器*/
/*TODO: damage.c, 用于处理碰撞的后效以及产生爆炸(需要新类explosion)*/

typedef struct
{
    double x;
    double y;
} Point;

typedef struct Angle
{
    Point range;
    struct Angle *next;
} Angle;

typedef struct Ship
{
    Point p;
    Point v;
    Point a;
    struct Ship *next;
    unsigned int id;
    int team;
} Ship;

typedef struct DamageSrc
{
    Point o;
    Point v;
    double r;
    double ev;
    Angle *disabled;
    struct DamageSrc *next;
} DamageSrc;

typedef struct
{
    Point a;
    double laser_direction;
    int detect;
    int explode;
} ShipAct;

typedef struct Plan
{
    struct Plan *next;
    struct Plan *prev;
    long tick;
    ShipAct act;
} Plan;

typedef struct Discovery
{
    Point p;
    struct Discovery *next;
    long tick;
    int observer_team;
} Discovery;

typedef struct
{
    double accurate_tick;
    Point damage;
} Collision;

typedef struct Square
{
    Point center;
    double side;
    struct Square *next;
} Square;

typedef struct
{
    char info;
    /*
    0xxxxxxx: it's a message (Discovery List)
    1xxxxxxx: it's a command (Plan List)
    xxxxxxx0: not gg yet
    xxxxxxx1: gg
    */
    long send_tick;
    void *data;
} Message;

typedef struct Monitored
{
    Ship *ship;
    Message *message;
    struct Monitored *next;
} Monitored;

typedef struct
{
    Plan plans[SHIP_CNT];
    Monitored monitors[SHIP_CNT];
} PlanController;

typedef struct
{
    Ship *ships;
    Square *squares;
    DamageSrc *lasers;
    DamageSrc *debris;
    Discovery *discoveries;
    long tick;
} Map;

int get_discovery(
    const Plan *observed_plan,
    const Ship *observer,
    const Ship *observed,
    long current_tick,
    long max_backtrace_tick,
    const Square *covering,
    int pierce,
    Discovery *out);

int get_collisions(
    const Ship *ship,
    const DamageSrc *damage,
    const Square *considered_covering,
    Collision *out);

void update_damagesrc(
    DamageSrc *damage,
    const Square *covering,
    Pool *pool_for_angles);

#endif
