#ifndef ANGLE_H
#define ANGLE_H

int direction_normalize(
    double direction
);

int is_in_angle(
    Angle*angle,
    double direction
);

int is_in_angles(
    Angle*angles,
    double direction
);

#endif