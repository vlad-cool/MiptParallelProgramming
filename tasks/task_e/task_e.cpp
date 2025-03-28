#include <iostream>
#include <iomanip>
#include <string.h>
#include <cmath>

#include <mpi.h>

#include "bigint.h"

// #define DEBUG_1

int backward_striling_formula(double n)
{
    double low = 1;
    double high = n;
    double mid = n;

    while (high - low > 0.0001)
    {
        mid = (low + high) / 2;
        double value = mid * std::log10(mid);

        if (value < n)
            low = mid;
        else
            high = mid;
    }

    return (int)mid;
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        std::cout << "No digits number provided" << std::endl;
        return 1;
    }
    long long power = atoi(argv[1]);

    BigInt precision = BigInt("1" + std::string(power, '0'));

    long long N = backward_striling_formula(power) * 2; // Number of summands

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
    if (my_rank == 0)
        from = 1;
    if (my_rank + 1 == commsize)
        to = N;
    else
        to = N / commsize * (my_rank + 1);

    BigInt inverted_factorial = precision;
    BigInt res = 0;

    for (unsigned int i = from; i < to; i++)
    {
        res = res + inverted_factorial;
        if (i != 0)
        {
            inverted_factorial = inverted_factorial / i;
        }
    }

    if (my_rank != 0)
    {
#ifdef DEBUG
        std::cout << my_rank << " Started recieving prev factorial" << std::endl;
#endif

        int buf_size;
        MPI_Recv(&buf_size, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD, &status);
        unsigned int *buf = new unsigned int[buf_size];
        MPI_Recv(buf, buf_size, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD, &status);

#ifdef DEBUG
        std::cout << my_rank << " Recieved buf with pref factorial" << std::endl;
#endif

        BigInt prev_factorial(buf, buf_size);
        delete[] buf;
        res = (res * prev_factorial).divide_power_10(power);
        inverted_factorial = (inverted_factorial * prev_factorial).divide_power_10(power);

#ifdef DEBUG
        std::cout << my_rank << " Recieved prev factorial" << std::endl;
#endif
    }

    if (my_rank + 1 != commsize)
    {
#ifdef DEBUG
        std::cout << my_rank << " Started sending factorial" << std::endl;
#endif
#ifdef DEBUG_1
        std::cout << inverted_factorial << std::endl;
#endif

        int buf_size = inverted_factorial.get_size();
        MPI_Send(&buf_size, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD);
        unsigned int *buf = inverted_factorial.get_data();
        MPI_Send(buf, buf_size, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD);

#ifdef DEBUG
        std::cout << my_rank << " Sent factorial" << std::endl;
#endif
    }

    if (my_rank == 0)
    {
        for (int i = 1; i < commsize; i++)
        {
#ifdef DEBUG
            std::cout << my_rank << " Started recieving res from " << i << std::endl;
#endif

            int buf_size;
            MPI_Recv(&buf_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            unsigned int *buf = new unsigned int[buf_size];
            MPI_Recv(buf, buf_size, MPI_INT, i, 0, MPI_COMM_WORLD, &status);

            res = res + BigInt(buf, buf_size);
            delete[] buf;

#ifdef DEBUG
            std::cout << my_rank << " Recieved res from " << i << std::endl;
#endif
        }

        std::cout << res << std::endl;
    }
    else
    {
#ifdef DEBUG
        std::cout << my_rank << " Started sending res" << std::endl;
#endif

        int buf_size = res.get_size();
        MPI_Send(&buf_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        unsigned int *buf = res.get_data();
        MPI_Send(buf, buf_size, MPI_INT, 0, 0, MPI_COMM_WORLD);

#ifdef DEBUG
        std::cout << my_rank << " Sent res" << std::endl;
#endif
    }

    MPI_Finalize();
}
