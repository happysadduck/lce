#ifndef POLY_H
#define POLY_H

int solve_quartic_roots(
    double a4, double a3,
    double a2, double a1, double a0,
    double *roots, double t
);

int solve_quadratic_roots(
    double a, double b,
    double c, double*roots, double t
);

#endif