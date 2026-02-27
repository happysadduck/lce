#include <math.h>
#include "core.h"
#include "config.h"

#define MAX_INTERVALS 100

static int solve_quadratic_roots(
    double a2, double a1, double a0,
    double t_max, double *roots)
{
    const double EPS = 1e-12;
    int cnt = 0;
    if (fabs(a2) < EPS)
    {
        if (fabs(a1) < EPS)
            return 0;
        double t = -a0 / a1;
        if (t >= -EPS && t <= t_max + EPS)
        {
            if (t < 0)
                t = 0;
            if (t > t_max)
                t = t_max;
            roots[cnt++] = t;
        }
        return cnt;
    }
    double discriminant = a1 * a1 - 4.0 * a2 * a0;
    if (discriminant < -EPS)
    {
        return 0;
    }
    if (discriminant < 0)
        discriminant = 0;
    double sqrt_d = sqrt(discriminant);
    double q = -0.5 * (a1 + copysign(sqrt_d, a1));
    double t1 = q / a2;
    double t2 = a0 / q;
    if (fabs(q) < EPS)
    {
        t2 = t1;
    }
    for (int i = 0; i < 2; i++)
    {
        double t = (i == 0) ? t1 : t2;
        if (t >= -EPS && t <= t_max + EPS)
        {
            if (
                cnt == 0 ||
                fabs(t - roots[cnt - 1]) > EPS)
                roots[cnt++] = t;
        }
    }
    return cnt;
}

int get_collision_tick(
    Ship *ship,
    DamageSrc *damage,
    double *out)
{
    ;
}

int get_collision(
    Ship *ship,
    DamageSrc *damage,
    Collision *out)
{
    ;
}