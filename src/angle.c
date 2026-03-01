#include"angle.h"
#include"core.h"

int direction_normalize(
    double direction
){
    while(direction<0)
        direction+=2.0*PI;
    while(direction>=2.0*PI)
        direction-=2.0*PI;
    return direction;
}

int is_in_angle(
    const Angle*angle,
    double direction
){
    if(angle->range.x<angle->range.y){
        return(
            direction>=angle->range.x &&
            direction<=angle->range.y
        );
    }
    return(
        direction>=angle->range.x ||
        direction<=angle->range.y
    );
}

int is_in_angles(
    const Angle*angles,
    double direction
){
    while(angles){
        if(is_in_angles(angles, direction))
            return 1;
        angles=angles->next;
    }
    return 0;
}

Angle*add_angle(
    Angle*angles,
    Angle*angle
){
    angle->next=angles;
    return angle;
}
