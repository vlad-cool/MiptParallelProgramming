#include <cmath>
#include <numbers>

#include "problem.h"

double x_step = 5e-3;
double t_step = 5e-3;

double a = 1;

double x_max = 100;
double t_max = 100;

double phi(double x)
{
    return std::sin(x/5) * x / 20;
}

double psi(double t)
{
    return 2*std::sin(t);
}

double f(double x, double t)
{
    return std::max(0.0, 100 - (x - 50) * (x - 50)) / 100;
}

// double f(double x, double t)
// {
//     // if (t > 0.25)
//     // {
//     //     return (t - 0.25) / 10;
//     // }
//     // else
//     // {
//     //     return 0;
//     // }
//     // return x;
//     // return 0;
//     return 0.3;
//     // return std::sin(x * t);
// }

// double phi(double x)
// {
//     return 0;
//     // return 1;
//     // return std::sin(x * std::numbers::pi * 4);
// }

// double psi(double t)
// {
//     // return 1;
//     // return 1 - t / 10;
//     // return t / 3;
//     // return 0;
//     return 1 - std::cos(t * 5 * std::numbers::pi);
//     // return t;
// }
