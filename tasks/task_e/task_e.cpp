#include <iostream>
#include <string.h>

#include <mpi.h>

#include "../../common/bigint.h"

int main(int argc, char *argv[])
{
    long long N = 10000; // Number of summands
    long long power = 10000; // Number of digits after dot

    bigint ten = bigint("10");
    bigint power_bi = bigint(power);
    bigint precision = big_pow(ten, power_bi);

    int commsize, my_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    MPI_Status status;

    long long from, to;

    from = N / commsize * my_rank;
    if (my_rank == 0)
        from = 1;
    if (my_rank + 1 == commsize)
        to = N;
    else
        to = from + N / commsize;

    bigint factorial = 1;
    bigint res = 0;

    for (long long i = from; i < to; i++)
    {
        res += precision / factorial;
        if (i != 0)
            factorial *= i;
        // std::cout << i << std::endl;
    }

    if (my_rank != 0)
    {
        std::cout << my_rank << " started recieving" << std::endl;
        int buf_size;
        MPI_Recv(&buf_size, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD, &status);
        char *buf = new char[buf_size];
        MPI_Recv(buf, buf_size, MPI_CHAR, my_rank - 1, 0, MPI_COMM_WORLD, &status);
        
        std::cout << my_rank << " recieved " << std::endl;
        bigint prev_factorial(buf);
        delete[] buf;
        res = res / prev_factorial;
        factorial *= prev_factorial;
    }

    std::cout << my_rank << " counted " << res / precision << "." << res % precision << " factorial " << factorial << std::endl;

    if (my_rank + 1 != commsize)
    {
        std::ostringstream oss;
        oss << factorial;
        int buf_size = oss.str().size() + 1;
        char *buf = new char[buf_size];
        strcpy(buf, oss.str().c_str());
        std::cout << my_rank << " started sending" << std::endl;
        MPI_Send(&buf_size, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD);
        MPI_Send(buf, buf_size, MPI_CHAR, my_rank + 1, 0, MPI_COMM_WORLD);
        delete[] buf;
    }

    // std::cout << 

    if (my_rank == 0)
    {
        for (int i = 1; i < commsize; i++)
        {
            // MPI_Recv(buf, power + 2, MPI_CHAR, my_rank - 1, 0, MPI_COMM_WORLD, &status);
            std::cout << "0 started recieveing from " << i << std::endl;
            char *buf = new char[power + 2];
            MPI_Recv(buf, power + 2, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
            res += bigint(buf);
            delete[] buf;
        }

        std::cout << "000  000   0000 0 0 0 00 0 recieved" << std::endl;

        // delete[] buf;
        
        std::cout << res / precision << "." << res % precision << std::endl;
    }
    else
    {
        std::ostringstream oss;
        oss << res;
        char *buf = new char[power + 2];
        std::cout << "size: " << oss.str().size() << std::endl;
        strcpy(buf, oss.str().c_str());
        std::cout << my_rank << " started sending" << std::endl;
        MPI_Send(buf, power + 2, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        delete[] buf;
    }

    MPI_Finalize();
}