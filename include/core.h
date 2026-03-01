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
   Ship*ships;
   DamageSrc*lasers;
   DamageSrc*debris;
}Map;

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

typedef struct{
   Plan plans[SHIP_CNT];
}PlanController;

typedef struct{
   Point p;
   long tick;
}Discovery;

typedef struct{
   double accurate_tick;
   Point damage;
}Collision;

typedef struct{
   Point center;
   double side;
}Square;

int get_discovery_general(
   const Plan*observed_plan,
   const Ship*observer,
   const Ship*observed,
   long current_tick,
   long max_backtrace_tick,
   Discovery*out
);

int get_collisions(
    Ship*ship,
    DamageSrc*damage,
    Square*considered_covering,
    Collision*out
);

void update_damagesrc(
    DamageSrc*damage,
    Square*covering,
    Pool*pool_for_angles
);

#endif
