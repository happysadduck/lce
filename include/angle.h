#ifndef ANGLE_H
#define ANGLE_H

#define PI 3.14159265358979323846

#include"core.h"

int direction_normalize(
    double direction
);

int is_in_angle(
    const Angle*angle,
    double direction
);

int is_in_angles(
    const Angle*angles,
    double direction
);

Angle*add_angle(
    Angle*angles,
    Angle*angle
);

#endif
