#ifndef CORE_H
#define CORE_H

#include"config.h"
#include"pool.h"

typedef struct{
    double x;
    double y;
}Point;

typedef struct Angle{
    Point range;
    struct Angle*next;
}Angle;

typedef struct Ship{
    Point p;
    Point v;
    Point a;
    struct Ship*next;
    unsigned int id;
}Ship;

typedef struct DamageSrc{
    Point o;
    Point v;
    double r;
    double ev;
    Angle*disabled;
    struct DamageSrc*next;
}DamageSrc;

typedef struct{
    Point a;
    double laser_direction;
    int detect;
    int explode;
}ShipAct;

typedef struct Plan{
    struct Plan*next;
    struct Plan*prev;
    long tick;
    ShipAct act;
}Plan;

typedef struct Discovery{
    Point p;
    long tick;
    struct Discovery*next;
}Discovery;

typedef struct{
    double accurate_tick;
    Point damage;
}Collision;

typedef struct{
    Point center;
    double side;
}Square;

typedef struct{
    char info;
    /*
    0xxxxxxx: it's a message (Discovery List)
    1xxxxxxx: it's a command (Plan List)
    xxxxxxx0: not gg yet
    xxxxxxx1: gg
    */
    void*data;
}Message;

typedef struct Monitored{
    Ship*ship;
    Message*message;
    struct Monitored*next;
}Monitored;

typedef struct{
    Plan plans[SHIP_CNT];
    Monitored monitors[SHIP_CNT];
}PlanController;

typedef struct{
    Ship*ships;
    DamageSrc*lasers;
    DamageSrc*debris;
    Discovery*discoveries;
    long tick;
}Map;

int get_discovery_general(
    const Plan*observed_plan,
    const Ship*observer,
    const Ship*observed,
    long current_tick,
    long max_backtrace_tick,
    Discovery*out
);

int get_collisions(
    const Ship*ship,
    const DamageSrc*damage,
    const Square*considered_covering,
    Collision*out
);

void update_damagesrc(
    DamageSrc*damage,
    const Square*covering,
    Pool*pool_for_angles
);

#endif
