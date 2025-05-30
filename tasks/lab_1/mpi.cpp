#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <numbers>
#include <chrono>
#include <mpi.h>

#include "problem.h"
#include "common.h"

int main(int argc, char *argv[])
{
    auto start = std::chrono::high_resolution_clock::now();
    int32_t size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int32_t x_n = static_cast<int32_t>(std::ceil(x_max / x_step));
    int32_t t_n = static_cast<int32_t>(std::ceil(t_max / t_step));

    int32_t chunk_x_size = 1000;
    int32_t chunk_t_size = 1000;

    int32_t chunk_x_n = (x_n + chunk_x_size - 1) / chunk_x_size;
    int32_t chunk_t_n = (t_n + chunk_t_size - 1) / chunk_t_size;

    chunk_x_n = (chunk_x_n + size - 1) / size * size;
    chunk_t_n = (chunk_t_n + size - 1) / size * size;

    x_n = chunk_x_n * chunk_x_size + 1;
    t_n = chunk_t_n * chunk_t_size + 1;

    x_step = x_max / x_n;
    t_step = t_max / t_n;

    x_step = x_step / a;

    double *buf = new double[x_n * t_n];
    double **field = new double *[t_n];
    for (int32_t i = 0; i < t_n; i++)
    {
        field[i] = buf + i * x_n;
    }

    for (int32_t t_i = 0; t_i < t_n; t_i++)
    {
        field[t_i][0] = psi(t_step * t_i);
    }
    for (int32_t x_i = 0; x_i < x_n; x_i++)
    {
        field[0][x_i] = phi(x_step * x_i);
    }

    int32_t chunk_number = 0;
    for (int32_t chunk_t_i = rank; chunk_t_i < chunk_t_n; chunk_t_i += size)
    {
        for (int32_t chunk_x_i = 0; chunk_x_i < chunk_x_n; chunk_x_i++)
        {
            if (rank == 0)
            {
                if (chunk_number >= size && chunk_number - size < (chunk_t_n * chunk_x_n / size - chunk_x_n))
                {
                    int32_t rec_x = chunk_x_i - size;
                    int32_t rec_t = chunk_t_i + size;

                    if (rec_x < 0)
                    {
                        rec_x += chunk_x_n;
                        rec_t -= size;
                    }

                    MPI_Recv(
                        field[rec_t * chunk_t_size] + rec_x * chunk_x_size + 1,
                        chunk_x_size,
                        MPI_DOUBLE,
                        size - 1, 0,
                        MPI_COMM_WORLD,
                        MPI_STATUS_IGNORE);
                }
            }
            else
            {
                MPI_Recv(
                    field[chunk_t_i * chunk_t_size] + chunk_x_i * chunk_x_size + 1,
                    chunk_x_size,
                    MPI_DOUBLE,
                    rank - 1,
                    0,
                    MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE);
            }

            rectangle_scheme(field, chunk_t_i * chunk_t_size + 1, chunk_t_size, chunk_x_i * chunk_x_size + 1, chunk_x_size);

            if (rank + 1 != size || chunk_t_i + 1 < chunk_t_n)
            {
                MPI_Send(
                    field[(chunk_t_i + 1) * chunk_t_size] + chunk_x_i * chunk_x_size + 1,
                    chunk_x_size,
                    MPI_DOUBLE,
                    (rank + 1) % size,
                    0,
                    MPI_COMM_WORLD);
            }

            chunk_number++;
        }
    }

    if (rank == 0)
    {
        for (int32_t i = 0; i < chunk_t_n; i++)
        {
            int32_t current_rank = i % size;
            if (current_rank == 0)
            {
                continue;
            }
            MPI_Recv(field[i * chunk_t_size + 1], chunk_t_size * x_n, MPI_DOUBLE, current_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
    else
    {
        for (int32_t i = rank; i < chunk_t_n; i += size)
        {
            MPI_Send(field[i * chunk_t_size + 1], chunk_t_size * x_n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    if (rank == 0)
    {
        std::cerr << "Proceses: " << size << " calculation time: " << duration.count() / 1000000 << "." << std::setfill('0') << std::setw(6) << duration.count() % 1000000 << " seconds" << std::endl;
    }

    if (rank == 0)
    {
        for (int32_t t_i = 0; t_i < t_n; t_i += 10)
        {
            for (int32_t x_i = 0; x_i < x_n; x_i += 10)
            {
                std::cout << std::setw(9) << std::showpos << std::fixed << std::setprecision(6) << field[t_i][x_i] << " ";
            }
            std::cout << std::endl;
        }
    }


    delete[] field[0];
    delete[] field;
    MPI_Finalize();
}
