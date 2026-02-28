#include<math.h>
#include"poly.h"

#define EPS 1e-12
#define MAX_ROOTS 4
#define SCAN_POINTS 100

static double quartic_val(
    double a4, double a3,
    double a2, double a1,
    double a0, double x)
{
    double result = a4;
    result = result * x + a3;
    result = result * x + a2;
    result = result * x + a1;
    return result * x + a0;
}

static double illinois_root(
    double a4, double a3, double a2,
    double a1, double a0, double a,
    double b, double fa, double fb
){
    double c, fc;
    int side=0;
    while(fabs(b-a)>EPS){
        c=(a*fb-b*fa)/(fb-fa);
        fc=quartic_val(
            a4, a3, a2, a1, a0, c
        );
        if(fa*fc<0){
            b=c;
            fb=fc;
            if(side==1)
                fa/=2;
            side=1;
        }else{
            a=c;
            fa=fc;
            if(side==2)
                fb/=2;
            side=2;
        }
        if(fc==0.0){
            a=b=c;
            break;
        }
    }
    return (a+b)/2.0;
}

static int is_duplicate(
    double x, double*roots,
    int cnt, double eps
){
    for(int i=0; i<cnt; i++){
        if(fabs(x-roots[i])<EPS*10)
            return 1;
    }
    return 0;
}

int solve_quartic_roots(
    double a4, double a3,
    double a2, double a1, double a0,
    double *roots, double t
){
    if(t<0) return 0;
    if(t==0){
        if(fabs(a0)<EPS){
            roots[0]=0.0;
            return 1;
        }return 0;
    }
    int root_cnt=0;
    double step=t/SCAN_POINTS;
    double x_prev=0.0;
    double f_prev=quartic_val(
        a4, a3, a2, a1, a0, x_prev
    );
    if(fabs(f_prev)<EPS)
        roots[root_cnt++]=0.0;
    for(
        int i=1;
        i<=SCAN_POINTS &&
        root_cnt<MAX_ROOTS;
        i++
    ){
        double x_curr=(i==SCAN_POINTS)
            ? t: i*step;
        double f_curr=quartic_val(
            a4, a3, a2, a1, a0, x_curr
        );
        if(f_prev*f_curr<0){
            double root=illinois_root(
                a4, a3, a2, a1, a0,
                x_prev, x_curr,
                f_prev, f_curr
            );
            if(!is_duplicate(
                root, roots, root_cnt, EPS
            )) roots[root_cnt++]=root;
        }else if(
            fabs(f_curr)<EPS &&
            !is_duplicate(
                x_curr, roots, 
                root_cnt, EPS
            )
        ) roots[root_cnt++]=x_curr;
        x_prev=x_curr;
        f_prev=f_curr;
    }
    if(root_cnt<MAX_ROOTS){
        double f_t=quartic_val(
            a4, a3, a2, a1, a0, t
        );
        if(
            fabs(f_t)<EPS &&
            !is_duplicate(
                t, roots,
                root_cnt, EPS
            )
        ) roots[root_cnt++]=t;
    }
    return root_cnt;
}

int solve_quadratic_roots(
    double a, double b,
    double c, double*roots, double t
){
    if(fabs(a)<EPS){
        if(fabs(b)<EPS) return 0;
        double x=-c/b;
        if(x>=-EPS && x<=t+EPS){
            roots[0]=x;
            return 1;
        }
        return 0;
    }
    double disc=b*b-4*a*c;
    if(disc<-EPS) return 0;
    if(fabs(disc)<EPS){
        double x=-b/(2*a);
        if(x>=-EPS && x<=t+EPS){
            roots[0]=x;
            return 1;
        }
        return 0;
    }
    double sqrt_d=sqrt(disc);
    double x1, x2;
    if(b>=0){
        x1=(-b-sqrt_d)/(2*a);
        x2=(2*c)/(-b-sqrt_d);
    }else{
        x1=(-b+sqrt_d)/(2*a);
        x2=(2*c)/(-b+sqrt_d);
    }
    int cnt=0;
    if(x1>=-EPS && x1<=t+EPS)
        roots[cnt++]=x1;
    if(x2>=-EPS && 
        x2<=t+EPS &&
        fabs(x2-x1)>EPS
    ) roots[cnt++]=x2;
    return cnt;
}