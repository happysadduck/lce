#include<math.h>
#include"core.h"
#include"config.h"

/*CAN BE FASTER*/
int get_discovery(
    const Ship*observer,
    const Ship*observed,
    long current_tick,
    long max_backtrace_tick,
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
            return 1;
        }
        prev_f=curr_f;
    }
    return 0;
}

int get_discovery_general(
    const Plan*observed_plan,
    const Ship*observer,
    const Ship*observed,
    long current_tick,
    long max_backtrace_tick,
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
            get_discovery(
                observer,
                &temp_ship,
                current_tick,
                L, out
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