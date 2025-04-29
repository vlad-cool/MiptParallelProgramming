#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <numbers>

#include "problem.h"
#include "common.h"

uint64_t **generate_workers(uint64_t chunk_t_n, uint64_t chunk_x_n)
{
    uint64_t *workers_tmp = new uint64_t[chunk_x_n * chunk_t_n];
    uint64_t **workers = new uint64_t *[chunk_t_n];
    for (int64_t i = 0; i < chunk_t_n; i++)
    {
        workers[i] = workers_tmp + i * chunk_x_n;
    }
    for (int64_t i = 0; i < chunk_x_n * chunk_t_n; i++)
    {
        workers_tmp[i] = 0;
    }

    uint64_t index = 0;
    for (int64_t i = 0; i < chunk_t_n && i < chunk_x_n; i++)
    {
        for (int64_t j = 0; j <= i; j++)
        {
            workers[i - j][j] = index;
            index++;
        }
    }
    for (int64_t i = chunk_x_n; i < chunk_t_n; i++)
    {
        for (int64_t j = 0; j < chunk_x_n; j++)
        {
            workers[i - j][j] = index;
            index++;
        }
    }
    for (int64_t i = chunk_t_n; i < chunk_x_n; i++)
    {
        for (int64_t j = chunk_t_n - 1; j >= 0; j--)
        {
            workers[j][i - j] = index;
            index++;
        }
    }
    if (chunk_x_n <= chunk_t_n)
    {
        for (int64_t i = 1; i < chunk_x_n; i++)
        {
            for (int64_t j = 0; j + i < chunk_x_n; j++)
            {
                workers[chunk_t_n - 1 - j][i + j] = index;
                index++;
            }
        }
    }
    else
    {
        for (int64_t i = 1; i < chunk_t_n; i++)
        {
            for (int64_t j = chunk_t_n - 1 - i; j >= 0; j--)
            {
                workers[i + j][chunk_x_n - 1 - j] = index;
                index++;
            }
        }
    }

    return workers;
}

int main(int argc, char *argv[])
{
    uint64_t x_n = static_cast<uint64_t>(std::ceil(x_max / x_step));
    uint64_t t_n = static_cast<uint64_t>(std::ceil(t_max / t_step));

    uint64_t chunk_x_size = 100;
    uint64_t chunk_t_size = 100;
    uint64_t chunk_x_n = (x_n + chunk_x_size - 1) / chunk_x_size;
    uint64_t chunk_t_n = (t_n + chunk_t_size - 1) / chunk_t_size;
    x_n = chunk_x_n * chunk_x_size;
    t_n = chunk_t_n * chunk_t_size;

    uint64_t a = 7;
    uint64_t b = 5;
    uint64_t **workers = generate_workers(a, b);
    std::cout << "Workers:\n";
    for (uint64_t t_i = 0; t_i < a; t_i++)
    {
        for (uint64_t x_i = 0; x_i < b; x_i++)
        {
            std::cout << std::setw(2) << workers[t_i][x_i] << " ";
        }
        std::cout << std::endl;
    }

    a = 5;
    b = 7;
    workers = generate_workers(a, b);
    std::cout << "Workers:\n";
    for (uint64_t t_i = 0; t_i < a; t_i++)
    {
        for (uint64_t x_i = 0; x_i < b; x_i++)
        {
            std::cout << std::setw(2) << workers[t_i][x_i] << " ";
        }
        std::cout << std::endl;
    }

    return 0;

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
    delete[] workers[0];
    delete[] workers;
    delete[] buf;
}
