#include"angle.h"
#include"core.h"

#define PI 3.14159265358979323846

int direction_normalize(
    double direction
){
    while(direction<0)
        direction+=2*PI;
    while(direction>=2*PI)
        direction-=2*PI;
    return direction;
}

int is_in_angle(
    Angle*angle,
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
    Angle*angles,
    double direction
){
    while(angles){
        if(is_in_angles(angles, direction))
            return 1;
        angles=angles->next;
    }
    return 0;
}