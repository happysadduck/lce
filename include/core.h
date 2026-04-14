#ifndef CORE_H
#define CORE_H

#include "config.h"
#include "pool.h"
#include "generic_hash_table.h"

/*TODO: 认为plan controller中的plan和MsgReceiver应该用哈希表比较好. 或需要id分配器*/
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

typedef struct ShipStructNode
{
    char part_type;
    /*
    m: main commander
    f: factory
    c: commander
    e: engine
    c: energy core
    s: store
    l: laser
    r: rader
    b: engineering robots
    */
    int state;
    /*
    engine: power
    s: front half for fuel storage, end half for materials storage
    */
    struct ShipStructNode *up;
    struct ShipStructNode *down;
    struct ShipStructNode *left;
    struct ShipStructNode *right;
} ShipStructNode;

typedef struct Ship
{
    Point p;
    Point v;
    Point a;
    int max_discovered_light_tick;
    ShipStructNode *commander_part;
    struct Ship *next;
    unsigned int id;
    struct Ship *king;
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
    int disappear;
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
    char is_command;
    char is_gg;
    long send_tick;
    void *data; /*ShipAct/Discovery*/
} Message;

typedef struct MsgReceiver
{
    Ship *msg_src;
    Message message;
    struct MsgReceiver *next;
} MsgReceiver;

typedef struct
{
    Pool *pool_for_discoveries;
    Pool *pool_for_ght_nodes;
    Pool *pool_for_msgs;
    ght_table_t plans;
    ght_table_t receivers;
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
    const Square *covers,
    int pierce,
    Discovery *out);

int get_collisions(
    const Ship *ship,
    const DamageSrc *damage,
    const Square *covers,
    Collision *out);

void update_damagesrc(
    DamageSrc *damage,
    const Square *covers,
    Pool *pool_for_angles);

int calculate_damage(Ship *ship, const Collision *collisions);

#endif
