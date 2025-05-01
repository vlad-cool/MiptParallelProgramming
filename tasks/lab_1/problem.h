#ifndef PROBLEM_H
#define PROBLEM_H

#include <cmath>
#include <numbers>

extern double x_step;
extern double t_step;

extern double a;

extern double x_max;
extern double t_max;

double f(double x, double t);
double phi(double x);
double psi(double t);

#endif
