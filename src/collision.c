#include <math.h>
#include"core.h"
#include"config.h"
#include"poly.h"
#include"angle.h"

static int get_collision_tick(
    const Ship*ship,
    const DamageSrc*damage,
    double*out)
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
    const DamageSrc*circle,
    const Square*square,
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
    const Ship*ship,
    const DamageSrc*damage,
    const Square*considered_covering,
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

static double binary_search_boundary(
    const DamageSrc*circle,
    const Square*square,
    double t1, double t2, int s1
){
    double left=t1;
    double right=t2;
    const double EPS=1e-12;
    if(right<left) right+=2.0*PI;
    while(right-left>EPS){
        double mid=(left+right)/2.0;
        double mid_norm=direction_normalize(mid);
        int mid_state=square_hinder(
            circle, square, mid_norm
        );
        if(mid_state==s1)
            left==mid;
        else right=mid;
    }
    return direction_normalize((left+right)/2.0);
}

void update_damagesrc(
    DamageSrc*damage,
    const Square*covering,
    Pool*pool_for_angles
){
    const int MAX_BOUNDARIES=16;
    const int COARSE_CNT=32;
    double boundaries[MAX_BOUNDARIES];
    int boundary_cnt=0;
    int coarse_status[COARSE_CNT];
    double coarse_angs[COARSE_CNT];
    for(int i=0; i<COARSE_CNT; i++){
        coarse_angs[i]=(i*2.0*PI)/COARSE_CNT;
        coarse_status[i]=square_hinder(
            damage, covering, coarse_angs[i]
        );
    }
    for(int i=0; i<COARSE_CNT; i++){
        int next=(i+1)%COARSE_CNT;
        double t1=coarse_angs[i];
        double t2=coarse_angs[next];
        int s1=coarse_status[i];
        int s2=coarse_status[next];
        if(s1!=s2){
            if(boundary_cnt<MAX_BOUNDARIES)
                boundaries[boundary_cnt++]=
                    binary_search_boundary(
                        damage, covering,
                        t1, t2, s1
                    );
        }else{
            double dt=t2-t1;
            if(dt<0) dt+=2.0*PI;
            if(dt>PI/8.0 && 
                boundary_cnt+1<MAX_BOUNDARIES
            ){
                double mid=direction_normalize(
                    t1+dt/2.0
                );
                int mid_state=square_hinder(
                    damage, covering, mid
                );
                if(mid_state!=s1){
                    boundaries[boundary_cnt++]=
                        binary_search_boundary(
                            damage, covering,
                            t1, mid, s1
                        );
                    boundaries[boundary_cnt++]=
                        binary_search_boundary(
                            damage, covering,
                            mid, t2, mid_state
                        );
                }
            }
        }
    }
    if(boundary_cnt==0){
        if(square_hinder(damage, covering, 0.0)){
            Angle*new_hinder1=pool_alloc(
                pool_for_angles
            );
            Angle*new_hinder2=pool_alloc(
                pool_for_angles
            );
            new_hinder1->range.x=0.0;
            new_hinder1->range.y=PI;
            new_hinder2->range.x=PI;
            new_hinder2->range.y=0;
            add_angle(
                damage->disabled, new_hinder1
            );
            add_angle(
                damage->disabled, new_hinder2
            );
        }
        return;
    }
    for(int i=0; i<boundary_cnt; i++){
        double start=boundaries[i];
        double end=boundaries[(i+1)%boundary_cnt];
        double mid;
        if(end>start)
            mid=(start+end)/2.0;
        else
            mid=direction_normalize(
                start+(end+2.0*PI-start)/2.0
            );
        if(square_hinder(
            damage, covering, mid
        )){
            Angle*new_hinder=pool_alloc(
                pool_for_angles
            );
            new_hinder->range.x=start;
            new_hinder->range.y=end;
            add_angle(
                damage->disabled, new_hinder
            );
        }
    }
}
