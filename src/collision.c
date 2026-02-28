#include <math.h>
#include"core.h"
#include"config.h"
#include"poly.h"
#include"angle.h"

static int get_collision_tick(
    Ship *ship,
    DamageSrc *damage,
    double *out)
{
    Point D={
        ship->p.x-damage->o.x,
        ship->p.y-damage->o.y
    };
    Point V={
        ship->v.x-damage->v.x,
        ship->v.y-damage->v.y
    };
    Point A=ship->a;
    double r0=damage->r;
    double ev=damage->ev;
    double t_max=TICK_STEP;
    const double eps=1e-12;
    double a_norm2=A.x*A.x+A.y*A.y;
    if(a_norm2<eps){
        double Aq=(
            V.x*V.x+V.y*V.y
        )-ev*ev;
        double Bq=2.0*(
            (D.x*V.x+D.y*V.y)-
            r0*ev
        );
        double Cq=(
            D.x*D.x+D.y*D.y
        )-r0*r0;
        return solve_quadratic_roots(
            Aq, Bq, Cq, out, t_max
        );
    }else{
        double a4=a_norm2;
        double a3=4.0*(V.x*A.x+V.y*A.y);
        double a2=4.0*(
            (D.x*A.x+D.y*A.y)+
            (V.x*V.x+V.y*V.y)-
            ev*ev
        );
        double a1=8.0*(
            (D.x*V.x+D.y*V.y)-r0*ev
        );
        double a0=5.0*(
            (D.x*D.x+D.y*D.y)-r0*r0
        );
        return solve_quartic_roots(
            a4, a3, a2, a1, a0, out, t_max
        );
    }
}

static int square_hinder(
    DamageSrc*circle,
    Square*square,
    double direction
){
    double ux=cos(direction);
    double uy=sin(direction);
    double x0=circle->o.x+circle->r*ux;
    double y0=circle->o.y+circle->r*uy;
    double vx=circle->v.x+circle->ev*ux;
    double vy=circle->v.y+circle->ev*uy;
    double half=square->side*0.5;
    double xmin=square->center.x-half;
    double xmax=square->center.x+half;
    double ymin=square->center.y-half;
    double ymax=square->center.y+half;
    const double INF=1e100;
    const double EPS=1e-12;
    double tx_low, tx_high;
    if(fabs(vx)<EPS){
        if(x0>=xmin-EPS && x0<=xmax+EPS){
            tx_low=-INF;
            tx_high=INF;
        }else return 0;
    }else{
        tx_low=(xmin-x0)/vx;
        tx_high=(xmax-x0)/vx;
        if(tx_low>tx_high){
            double tmp=tx_low;
            tx_low=tx_high;
            tx_high=tmp;
        }
    }
    double ty_low, ty_high;
    if(fabs(vy)<EPS){
        if(y0>=ymin-EPS && y0<=ymax+EPS){
            ty_low=-INF;
            ty_high=INF;
        }else return 0;
    }else{
        ty_low=(ymin-y0)/vy;
        ty_high=(ymax-y0)/vy;
        if(ty_low>ty_high){
            double tmp=ty_low;
            ty_low=ty_high;
            ty_high=tmp;
        }
    }
    double t_start=fmax(tx_low, ty_low);
    double t_end=fmax(tx_high, ty_high);
    double overlap_start=fmax(t_start, 0.0);
    double overlap_end=fmin(t_end, TICK_STEP);
    if(overlap_start<=overlap_end+EPS)
        return 1;
    else return 0;
}

int get_collisions(
    Ship*ship,
    DamageSrc*damage,
    Square*considered_covering,
    Collision*out
){
    double collision_ticks[4];
    int out_cnt=0;
    int collision_cnt=get_collision_tick(
        ship, damage, collision_ticks
    );
    for(int i=0; i<collision_cnt; i++){
        double t=collision_ticks[i];
        double px=ship->p.x+
            ship->v.x*t+
            ship->a.x*t*t*0.5;
        double py=ship->p.y+
            ship->v.y*t+
            ship->a.y*t*t*0.5;
        double ox=damage->o.x+damage->v.x;
        double oy=damage->o.y+damage->v.y;
        double direction=direction_normalize(
            atan2(py-oy, px-ox));
        if(
            !is_in_angles(
                damage->disabled,
                direction
            ) && !square_hinder(
                damage, considered_covering,
                direction
            )
        ){
            out[out_cnt].accurate_tick=t;
            double vx=damage->v.x+
                damage->ev*cos(direction);
            double vy=damage->v.y+
                damage->ev*sin(direction);
            Point relative_v={
                vx-ship->v.x,
                vy-ship->v.y
            };
            out[out_cnt++].damage=relative_v;
        }
    }
    return out_cnt;
}