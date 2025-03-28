#include <iostream>
#include <iomanip>
#include <string.h>
#include <cmath>

#include <mpi.h>

#include "../../common/bigint.h"

// #define DEBUG

int backward_striling_formula(double n)
{
    double low = 1;
    double high = n;
    double mid;

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

    // bigint two = bigint("2");
    bigint ten = bigint("10");
    bigint power_bi = bigint(power);

    bigint precision = big_pow(ten, power_bi);

    long long N = backward_striling_formula(power) * 2; // Number of summands
    std::cout << N << std::endl;

    int commsize, my_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == 0)
    {
        std::cout << "N: " << N << std::endl;
        std::cout << "Power: " << power << std::endl;
    }

#ifdef DEBUG
    std::cout << std::setfill('0');
    std::cout << commsize << " " << my_rank << std::endl;
#endif

    MPI_Status status;

    long long from, to;

    from = N / commsize * my_rank;
    if (my_rank == 0)
        from = 1;
    if (my_rank + 1 == commsize)
        to = N;
    else
        to = N / commsize * (my_rank + 1);

#ifdef DEBUG
    std::cout << my_rank << " " << from << " " << to << std::endl;
#endif

    // bigint factorial = 1;
    bigint inverted_factorial = precision;
    bigint res = 0;

    for (long long i = from; i < to; i++)
    {
        res += inverted_factorial;
        if (i != 0)
        {
            inverted_factorial /= bigint(i);
        }
        std::cout << i << std::endl;

#ifdef DEBUG_2
        std::cout << i << std::endl;
        std::cout << res / precision << "." << res % precision << std::endl;
        std::cout << precision / factorial << std::endl;
#endif
    }

    if (my_rank != 0)
    {
#ifdef DEBUG
        std::cout << my_rank << " started recieving" << std::endl;
#endif
        int buf_size;
        MPI_Recv(&buf_size, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD, &status);
        char *buf = new char[buf_size];
        MPI_Recv(buf, buf_size, MPI_CHAR, my_rank - 1, 0, MPI_COMM_WORLD, &status);

#ifdef DEBUG
        std::cout << my_rank << " recieved " << std::endl;
#endif
        bigint prev_factorial(buf);
        delete[] buf;
        res = res * prev_factorial / precision;
        inverted_factorial *= prev_factorial / precision;
    }

#ifdef DEBUG
    std::cout << my_rank << " counted " << res / precision << "." << std::setw(power) << res % precision << " inverted factorial " << inverted_factorial / precision << std::setw(power) << inverted_factorial % precision << std::endl;
#endif

    if (my_rank + 1 != commsize)
    {
        std::ostringstream oss;
        oss << inverted_factorial;
        int buf_size = oss.str().size() + 1;
        char *buf = new char[buf_size];
        strcpy(buf, oss.str().c_str());

#ifdef DEBUG
        std::cout << my_rank << " started sending" << std::endl;
#endif
        MPI_Send(&buf_size, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD);
        MPI_Send(buf, buf_size, MPI_CHAR, my_rank + 1, 0, MPI_COMM_WORLD);
        delete[] buf;
    }

    if (my_rank == 0)
    {
        for (int i = 1; i < commsize; i++)
        {
#ifdef DEBUG
            std::cout << "0 started recieveing from " << i << std::endl;
#endif
            char *buf = new char[power + 2];
            MPI_Recv(buf, power + 2, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
            res += bigint(buf);
            delete[] buf;
        }

#ifdef DEBUG
        std::cout << "recieved" << std::endl;
#endif

        std::cout << res / precision << "." << res % precision << std::endl;
    }
    else
    {
        std::ostringstream oss;
        oss << res;
        char *buf = new char[power + 2];
#ifdef DEBUG
        std::cout << "size: " << oss.str().size() << std::endl;
#endif
        strcpy(buf, oss.str().c_str());
#ifdef DEBUG
        std::cout << my_rank << " started sending" << std::endl;
#endif
        MPI_Send(buf, power + 2, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        delete[] buf;
    }

    MPI_Finalize();
}