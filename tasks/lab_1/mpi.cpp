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

// int32_t **generate_workers(int32_t chunk_x_n, int32_t chunk_t_n)
// {
//     int32_t **workers = new int32_t *[chunk_t_n];
//     workers[0] = new int32_t[chunk_x_n * chunk_t_n];
//     for (int32_t i = 1; i < chunk_t_n; i++)
//     {
//         workers[i] = workers[0] + i * chunk_x_n;
//     }
//     for (int32_t i = 0; i < chunk_x_n * chunk_t_n; i++)
//     {
//         workers[0][i] = 0;
//     }

//     int32_t index = 0;
//     for (int32_t i = 0; i < chunk_t_n && i < chunk_x_n; i++)
//     {
//         for (int32_t j = 0; j <= i; j++)
//         {
//             workers[i - j][j] = index;
//             index++;
//         }
//     }
//     for (int32_t i = chunk_x_n; i < chunk_t_n; i++)
//     {
//         for (int32_t j = 0; j < chunk_x_n; j++)
//         {
//             workers[i - j][j] = index;
//             index++;
//         }
//     }
//     for (int32_t i = chunk_t_n; i < chunk_x_n; i++)
//     {
//         for (int32_t j = chunk_t_n - 1; j >= 0; j--)
//         {
//             workers[j][i - j] = index;
//             index++;
//         }
//     }
//     if (chunk_x_n <= chunk_t_n)
//     {
//         for (int32_t i = 1; i < chunk_x_n; i++)
//         {
//             for (int32_t j = 0; j + i < chunk_x_n; j++)
//             {
//                 workers[chunk_t_n - 1 - j][i + j] = index;
//                 index++;
//             }
//         }
//     }
//     else
//     {
//         for (int32_t i = 1; i < chunk_t_n; i++)
//         {
//             for (int32_t j = chunk_t_n - 1 - i; j >= 0; j--)
//             {
//                 workers[i + j][chunk_x_n - 1 - j] = index;
//                 index++;
//             }
//         }
//     }

//     return workers;
// }

int main(int argc, char *argv[])
{

    int32_t size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // MPI_File fh;
    // MPI_File_open(MPI_COMM_WORLD, "debug.log",
    //               MPI_MODE_CREATE | MPI_MODE_WRONLY,
    //               MPI_INFO_NULL, &fh);

    int32_t x_n = static_cast<int32_t>(std::ceil(x_max / x_step));
    int32_t t_n = static_cast<int32_t>(std::ceil(t_max / t_step));

    int32_t chunk_x_size = 100;
    int32_t chunk_t_size = 100;

    int32_t chunk_x_n = (x_n + chunk_x_size - 1) / chunk_x_size;
    int32_t chunk_t_n = (t_n + chunk_t_size - 1) / chunk_t_size;

    chunk_x_n = (chunk_x_n + size - 1) / size * size;
    chunk_t_n = (chunk_t_n + size - 1) / size * size;

    x_n = chunk_x_n * chunk_x_size + 1;
    t_n = chunk_t_n * chunk_t_size + 1;

    // int32_t **workers = generate_workers(chunk_x_n, chunk_t_n);

    x_step = x_max / x_n;
    t_step = t_max / t_n;

    double **field = new double *[t_n];
    field[0] = new double[x_n * t_n];
    for (int32_t i = 1; i < t_n; i++)
    {
        field[i] = field[0] + i * x_n;
    }

    for (int32_t t_i = 0; t_i < t_n; t_i++)
    {
        field[t_i][0] = psi(t_step * t_i);
    }
    for (int32_t x_i = 0; x_i < x_n; x_i++)
    {
        field[0][x_i] = phi(x_step * x_i);
    }

    // int32_t *buf = new int32_t[chunk_x_size];

    // int32_t send_counter = 0;
    // int32_t recieve_counter = 0;

    int32_t chunk_number = 0;
    for (int32_t chunk_t_i = rank; chunk_t_i < chunk_t_n; chunk_t_i += size)
    {
        for (int32_t chunk_x_i = 0; chunk_x_i < chunk_x_n; chunk_x_i++)
        {
            // char msg[100];
            // int len;

            if (rank == 0)
            {
                // bool recieve = true;
                // recieve = recieve && (chunk_x_i + chunk_t_i * chunk_x_n) >= size;
                // recieve = recieve && ((chunk_t_i + 1) * chunk_t_size < chunk_t_n);
                // std::cout << chunk_number << " " << chunk_t_i << " " << chunk_t_n << " " << chunk_x_i << " " << chunk_x_n << std::endl;
                // std::cout << t_n << " " << x_n << std::endl;

                // if (chunk_number >= size && chunk_number - size < (chunk_t_n - 1) * (chunk_x_n))
                if (chunk_number >= size && chunk_number - size < (chunk_t_n * chunk_x_n / size - chunk_x_n))
                {
                    // len = sprintf(msg, "Rank %d started recieving\n", rank);
                    // MPI_File_write(fh, msg, len, MPI_CHAR, MPI_STATUS_IGNORE);

                    // std::cout << static_cast<char>(rank << 4 | 0) << std::endl;
                    // std::cout << 'a' << std::endl;
                    MPI_Recv(
                        field[(chunk_t_i + 1) * chunk_t_size] + (chunk_x_i + chunk_x_n - size) % chunk_x_n,
                        // buf,
                        chunk_x_size,
                        MPI_DOUBLE,
                        size - 1, 0,
                        MPI_COMM_WORLD,
                        MPI_STATUS_IGNORE);
                    // recieve_counter++;

                    // len = sprintf(msg, "Rank %d recieved\n", rank);
                    // MPI_File_write(fh, msg, len, MPI_CHAR, MPI_STATUS_IGNORE);
                    // std::cout << 'b' << std::endl;
                    // std::cout << static_cast<char>(rank << 4 | 1) << std::endl;
                }
            }
            else
            {
                // len = sprintf(msg, "Rank %d started recieving\n", rank);
                // MPI_File_write(fh, msg, len, MPI_CHAR, MPI_STATUS_IGNORE);

                // std::cout << static_cast<char>(rank << 4 | 0) << std::endl;

                MPI_Recv(
                    field[chunk_t_i * chunk_t_size] + chunk_x_i * chunk_x_size,
                    // buf,
                    chunk_x_size,
                    MPI_DOUBLE,
                    rank - 1,
                    0,
                    MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE);

                // len = sprintf(msg, "Rank %d recieved\n", rank);
                // MPI_File_write(fh, msg, len, MPI_CHAR, MPI_STATUS_IGNORE);

                // std::cout << static_cast<char>(rank << 4 | 1) << std::endl;

                // recieve_counter++;
            }

            rectangle_scheme(field, chunk_t_i * chunk_t_size + 1, chunk_t_size, chunk_x_i * chunk_x_size + 1, chunk_x_size);

            if (rank + 1 != size || chunk_t_i + 1 < chunk_t_n)
            {
                // len = sprintf(msg, "Rank %d started sending\n", rank);
                // MPI_File_write(fh, msg, len, MPI_CHAR, MPI_STATUS_IGNORE);

                // std::cout << static_cast<char>(rank << 4 | 2) << std::endl;
                MPI_Send(
                    field[(chunk_t_i + 1) * chunk_t_size - 1] + chunk_x_n * chunk_x_size,
                    // buf,
                    chunk_x_size,
                    MPI_DOUBLE,
                    (rank + 1) % size,
                    0,
                    MPI_COMM_WORLD);

                // send_counter++;

                // len = sprintf(msg, "Rank %d sended\n", rank);
                // MPI_File_write(fh, msg, len, MPI_CHAR, MPI_STATUS_IGNORE);
                // std::cout << static_cast<char>(rank << 4 | 3) << std::endl;
            }

            chunk_number++;
        }
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
    else
    {
    }

#ifdef DEBUG
    for (int32_t i = 0; i < x_n * t_n)
    {
    }
#endif

    // char msg[100];
    // int len;

    // len = sprintf(msg, "Rank %d sended %d, recieved %d\n", rank, send_counter, recieve_counter);
    // MPI_File_write_ordered(fh, msg, len, MPI_CHAR, MPI_STATUS_IGNORE);
    // std::cout << static_cast<char>(rank << 4 | 3) << std::endl;

    delete[] field[0];
    delete[] field;
    // delete[] workers[0];
    // delete[] workers;

    // MPI_File_close(&fh);
    MPI_Finalize();
}
