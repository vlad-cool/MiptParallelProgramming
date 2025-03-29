#include <iostream>
#include <iomanip>
#include <string.h>
#include <cmath>

#include <mpi.h>

#include "bigint.h"

#define EXTRA_DGITS 10

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        std::cout << "No digits number provided" << std::endl;
        return 1;
    }
    long long power = atoi(argv[1]) + EXTRA_DGITS;

    BigInt precision = BigInt("1" + std::string(power, '0'));

    long long N = power; // Number of summands

    int commsize, my_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == 0)
    {
        std::cout << "N: " << N << std::endl;
        std::cout << "Power: " << power << std::endl;
    }

    MPI_Status status;

    long long from, to;

    from = N / commsize * my_rank;
    if (my_rank + 1 == commsize)
        to = N;
    else
        to = N / commsize * (my_rank + 1);

    BigInt inverted_factorial = precision;
    BigInt res = 0;

    for (unsigned int i = from; i < to; i++)
    {
        if (i != 0)
        {
            inverted_factorial = inverted_factorial / i;
        }
        res = res + inverted_factorial;
    }

    if (my_rank + 1 != commsize)
    {
        int buf_size;
        MPI_Recv(&buf_size, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD, &status);
        unsigned int *buf = new unsigned int[buf_size];
        MPI_Recv(buf, buf_size, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD, &status);

        BigInt next_res(buf, buf_size);
        delete[] buf;
        res = (next_res * inverted_factorial).divide_power_10(power) + res;
    }

    if (my_rank != 0)
    {
        int buf_size = res.get_size();
        MPI_Send(&buf_size, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD);
        unsigned int *buf = res.get_data();
        MPI_Send(buf, buf_size, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD);
    }

    if (my_rank == 0)
    {
        std::cout << res.divide_power_10(EXTRA_DGITS) << std::endl;
    }
    
    MPI_Finalize();
}
