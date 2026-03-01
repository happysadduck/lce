#include<math.h>
#include"core.h"
#include"config.h"

const int square_segment_intersect(
    const Square*square,
    const Point*s1,
    const Point*s2
){
    double half=square->side/2.0;
    double side[]={
        square->center.x-half,
        square->center.x+half,
        square->center.y-half,
        square->center.y+half
    };
    if(
        s1->x>=side[0] && s1->x<=side[1] &&
        s1->y>=side[2] && s1->y<=side[3]
    ) return 1;
    if(
        s2->x>=side[0] && s2->x <=side[1] &&
        s2->y>=side[2] && s2->y<=side[3]
    ) return 1;
    double bd[]={
        s1->x, s2->x,
        s1->y, s2->y
    };
    for(int i=0; i<2; i++){
        for(int j=0; j<2; j++){
            if(bd[2*(!i)]!=bd[2*(!i)+1]){
                double t=
                    (side[2*(!i)+j]-bd[2*(!i)])/
                    (bd[2*(!i)+1]-bd[2*(!i)]);
                if(t>=0 && t<=1){
                    double b=bd[2*i]+t*
                        (bd[2*i+1]-bd[2*i]);
                    if(b>=side[2*i] && 
                        b<=side[2*i+1]
                    ) return 1;
                }
            }else if(bd[2*(!i)]==side[2*i+j]){
                if(
                    fmax(bd[2*i], bd[2*i+1])
                        >=side[2*i] &&
                    fmin(bd[2*i], bd[2*i+1])
                        <=side[2*i+1]
                ) return 1;
            }
        }
    }
    return 0;
}

/*CAN BE FASTER*/
static get_discovery_simple(
    const Ship*observer,
    const Ship*observed,
    long current_tick,
    long max_backtrace_tick,
    const Square*covering,
    int pierce,
    Discovery*out
){
    double Ox=observer->p.x;
    double Oy=observer->p.y;
    double Bx=observed->p.x;
    double By=observed->p.y;
    double Vx=observed->v.x;
    double Vy=observed->v.y;
    double Ax=observed->a.x;
    double Ay=observed->a.y;
    double dist0=sqrt(
        (Ox-Bx)*(Ox-Bx)+(Oy-By)*(Oy-By)
    );
    double prev_f=dist0;
    if(current_tick==max_backtrace_tick)
        return 0;
    for(
        long t=current_tick-1;
        t>=max_backtrace_tick;
        t--
    ){
        long dt_ticks=current_tick-t;
        double dt_sec=dt_ticks*TICK_STEP;
        double px=Bx-Vx*dt_sec
            -0.5*Ax*dt_sec*dt_sec;
        double py=By-Vy*dt_sec
            -0.5*Ay*dt_sec*dt_sec;
        double dx=Ox-px;
        double dy=Oy-py;
        double dist=sqrt(dx*dx+dy*dy);
        double curr_f=dist-dt_sec;
        if(curr_f<0){
            out->tick=t+1;
            double dt1_sec=(
                current_tick-(t+1)
            )*TICK_STEP;
            out->p.x=Bx-Vx*dt1_sec
                -0.5*Ax*dt1_sec*dt1_sec;
            out->p.y=By-Vy*dt1_sec
                -0.5*Ay*dt1_sec*dt1_sec;
            if(pierce ||
                !square_segment_intersect(
                covering, &out->p, &observer->p
            )) return 1;
            return 0;
        }
        prev_f=curr_f;
    }
    return 0;
}

int get_discovery(
    const Plan*observed_plan,
    const Ship*observer,
    const Ship*observed,
    long current_tick,
    long max_backtrace_tick,
    const Square*covering,
    int pierce,
    Discovery*out
){
    if(current_tick<=max_backtrace_tick)
        return 0;
    const Plan*curr_plan=observed_plan;
    while(
        curr_plan &&
        curr_plan->tick>current_tick
    ) curr_plan=curr_plan->prev;
    while(
        curr_plan &&
        curr_plan->next &&
        curr_plan->next->tick<=current_tick
    ) curr_plan=curr_plan->next;
    Point p_curr=observed->p;
    Point v_curr=observed->v;
    Point a_curr;
    if(curr_plan)
        a_curr=curr_plan->act.a;
    else{
        Point vec_zero={0,0};
        a_curr=vec_zero;
    }
    long R=current_tick;
    while(R>max_backtrace_tick){
        long L=max_backtrace_tick;
        if(curr_plan && curr_plan->prev){
            L=curr_plan->prev->tick;
            if(L<max_backtrace_tick)
                L=max_backtrace_tick;
        }
        Ship temp_ship;
        temp_ship.p=p_curr;
        temp_ship.v=v_curr;
        temp_ship.a=a_curr;
        temp_ship.next=0;
        if(
            get_discovery_simple(
                observer,
                &temp_ship,
                current_tick,
                L, covering, 
                pierce, out
            )
        ){
            if(
                out->tick>=L &&
                out->tick<=R
            ) return 1;
        }
        if(L==max_backtrace_tick)
            break;
        double dt_sec=(R-L)*TICK_STEP;
        Point pL, vL;
        pL.x=p_curr.x-v_curr.x*dt_sec
            -0.5*a_curr.x*dt_sec*dt_sec;
        pL.y=p_curr.y-v_curr.y*dt_sec
            -0.5*a_curr.y*dt_sec*dt_sec;
        vL.x=v_curr.x-a_curr.x*dt_sec;
        vL.y=v_curr.y-a_curr.y*dt_sec;
        p_curr=pL;
        v_curr=vL;
        R=L;
        curr_plan=curr_plan
            ? curr_plan->prev: 0;
        if(curr_plan)
            a_curr=curr_plan->act.a;
        else{
            Point vec_zero={0,0};
            a_curr=vec_zero;
        }
    }
    return 0;
}
