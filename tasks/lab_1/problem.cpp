#include <cmath>
#include <numbers>

#include "problem.h"

double x_step = 1e-3;
double t_step = 1e-3;

double x_max = 10;
double t_max = 10;

double f(double x, double t)
{
    // if (t > 0.25)
    // {
    //     return (t - 0.25) / 10;
    // }
    // else
    // {
    //     return 0;
    // }
    // return x;
    // return 0;
    return 0.3;
    // return std::sin(x * t);
}

double phi(double x)
{
    // return 0;
    // return 1;
    return std::sin(x * std::numbers::pi * 4);
}

double psi(double t)
{
    return t / 3;
    // return 0;
    // return 1 - std::cos(t / 5);
    // return t;
}
