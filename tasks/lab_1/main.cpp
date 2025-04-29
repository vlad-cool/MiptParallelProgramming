#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdint>
#include <cstddef>
#include <cassert>

// #define DEBUG

double x_step = 1e-3;
double t_step = 1e-3;

double x_max = 10;
double t_max = 5;

double f(double x, double t)
{
    return std::sin(x * t);
}

double phi(double x)
{
    return std::sin(x / 10);
}

double psi(double t)
{
    return t / 10;
    // return 0;
    // return 1 - std::cos(t / 5);
}

void rectangle_step(double **field, uint64_t x_i, uint64_t t_i)
{
#ifdef DEBUG
    assert(field[t_i][x_i] == 0.0);
    assert(field[t_i + 1][x_i] == 0.0);
    assert(field[t_i][x_i + 1] == 0.0);
#endif
    field[t_i][x_i] = 0;
    field[t_i][x_i] += 2 * x_step * t_step * f((x_i - 0.5) * x_step, (t_i - 0.5) * t_step);
    field[t_i][x_i] += x_step * (field[t_i - 1][x_i] - field[t_i][x_i - 1] + field[t_i - 1][x_i - 1]);
    field[t_i][x_i] += t_step * (field[t_i][x_i - 1] - field[t_i - 1][x_i] + field[t_i - 1][x_i - 1]);
    field[t_i][x_i] /= (x_step + t_step);
}

void rectangle_scheme(double **field, uint64_t t_0, uint64_t t_n, uint64_t x_0, uint64_t x_n)
{
    for (uint64_t t_i = t_0; t_i < t_n; t_i++)
    {
        for (uint64_t x_i = x_0; x_i < x_n; x_i++)
        {
            rectangle_step(field, x_i, t_i);
        }
    }
}

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

    rectangle_scheme(field, 1, t_n, 1, x_n);

    for (uint64_t t_i = 0; t_i < t_n; t_i += 10)
    {
        for (uint64_t x_i = 0; x_i < x_n; x_i += 10)
        {
            std::cout << std::setw(8) << field[t_i][x_i] << " ";
        }
        std::cout << std::endl;
    };

#ifdef DEBUG
    for (uint64_t i = 0; i < x_n * t_n)
    {
    }
#endif

    delete[] field;
    delete[] buf;
}
