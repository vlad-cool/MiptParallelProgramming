#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <numbers>
#include <mpi.h>

#include "problem.h"
#include "common.h"

uint64_t **generate_workers(uint64_t chunk_x_n, uint64_t chunk_t_n)
{
    uint64_t **workers = new uint64_t *[chunk_t_n];
    workers[0] = new uint64_t[chunk_x_n * chunk_t_n];
    for (int64_t i = 1; i < chunk_t_n; i++)
    {
        workers[i] = workers[0] + i * chunk_x_n;
    }
    for (int64_t i = 0; i < chunk_x_n * chunk_t_n; i++)
    {
        workers[0][i] = 0;
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
    int32_t size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    uint64_t x_n = static_cast<uint64_t>(std::ceil(x_max / x_step));
    uint64_t t_n = static_cast<uint64_t>(std::ceil(t_max / t_step));

    uint64_t chunk_x_size = 100;
    uint64_t chunk_t_size = 100;

    uint64_t chunk_x_n = (x_n + chunk_x_size - 1) / chunk_x_size;
    uint64_t chunk_t_n = (t_n + chunk_t_size - 1) / chunk_t_size;

    chunk_x_n = (chunk_x_n + size - 1) / size * size;
    chunk_t_n = (chunk_t_n + size - 1) / size * size;

    x_n = chunk_x_n * chunk_x_size + 1;
    t_n = chunk_t_n * chunk_t_size + 1;

    uint64_t **workers = generate_workers(chunk_x_n, chunk_t_n);

    x_step = x_max / x_n;
    t_step = t_max / t_n;

    double **field = new double *[t_n];
    field[0] = new double[x_n * t_n];
    for (uint64_t i = 1; i < t_n; i++)
    {
        field[i] = field[0] + i * x_n;
    }

    for (uint64_t t_i = 0; t_i < t_n; t_i++)
    {
        field[t_i][0] = psi(t_step * t_i);
    }
    for (uint64_t x_i = 0; x_i < x_n; x_i++)
    {
        field[0][x_i] = phi(x_step * x_i);
    }

    // uint64_t *buf = new uint64_t[chunk_t_size];

    MPI_Status mpi_status;

    uint64_t chunk_number = 0;
    for (uint64_t chunk_t_i = rank; chunk_t_i < chunk_t_n; chunk_t_i += size)
    {
        for (uint64_t chunk_x_i = 0; chunk_x_i < chunk_x_n; chunk_x_i++)
        {
            if (rank == 0)
            {
                if (chunk_number >= size && chunk_number + chunk_t_n + size < chunk_x_n * chunk_t_n / size)
                {
                    MPI_Recv(
                        field[(chunk_t_i + 1) * chunk_t_size] + (chunk_x_i + chunk_x_n - size) % chunk_x_n,
                        chunk_x_size,
                        MPI_DOUBLE,
                        size - 1, 0,
                        MPI_COMM_WORLD,
                        &mpi_status);
                }
            }
            else
            {
                if (chunk_t_i + size < chunk_t_n)
                {
                    MPI_Recv(
                        field[chunk_t_i * chunk_t_size] + chunk_x_i * chunk_x_size,
                        chunk_x_size,
                        MPI_DOUBLE,
                        rank - 1,
                        0,
                        MPI_COMM_WORLD,
                        &mpi_status);
                }
            }

            rectangle_scheme(field, chunk_t_i * chunk_t_size + 1, chunk_t_size, chunk_x_i * chunk_x_size + 1, chunk_x_size);

            if (chunk_t_i + size < chunk_t_n)
            {
                MPI_Send(
                    field[(chunk_t_i + 1) * chunk_t_size - 1] + chunk_x_n * chunk_x_size,
                    chunk_x_size,
                    MPI_DOUBLE,
                    (rank + 1) % size,
                    0,
                    MPI_COMM_WORLD
                );
            }

            chunk_number++;
        }
    }

    if (rank == 0)
    {
        for (uint64_t t_i = 0; t_i < t_n; t_i += 10)
        {
            for (uint64_t x_i = 0; x_i < x_n; x_i += 10)
            {
                std::cout << std::setw(8) << field[t_i][x_i] << " ";
            }
            std::cout << std::endl;
        }
    }
    else
    {
    }

#ifdef DEBUG
    for (uint64_t i = 0; i < x_n * t_n)
    {
    }
#endif

    delete[] field[0];
    delete[] field;
    delete[] workers[0];
    delete[] workers;

    MPI_Finalize();
}
