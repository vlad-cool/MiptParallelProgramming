#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <numbers>

#include "problem.h"
#include "common.h"

int main(int argc, char *argv[])
{
    uint64_t x_n = static_cast<uint64_t>(std::ceil(x_max / x_step));
    uint64_t t_n = static_cast<uint64_t>(std::ceil(t_max / t_step));
    x_step = x_max / x_n;
    t_step = t_max / t_n;

    double *buf = new double[x_n * t_n];
    double **field = new double *[t_n];
    for (uint64_t i = 0; i < t_n; i++)
    {
        field[i] = buf + i * x_n;
    }

    for (uint64_t t_i = 0; t_i < t_n; t_i++)
    {
        field[t_i][0] = psi(t_step * t_i);
    }
    for (uint64_t x_i = 0; x_i < x_n; x_i++)
    {
        field[0][x_i] = phi(x_step * x_i);
    }

    rectangle_scheme(field, 1, t_n - 1, 1, x_n - 1);

    for (uint64_t t_i = 0; t_i < t_n; t_i += 10)
    {
        for (uint64_t x_i = 0; x_i < x_n; x_i += 10)
        {
            std::cout << std::setw(9) << std::showpos << std::fixed << std::setprecision(6) << field[t_i][x_i] << " ";
        }
        std::cout << std::endl;
    };

    delete[] field;
    delete[] buf;
}
