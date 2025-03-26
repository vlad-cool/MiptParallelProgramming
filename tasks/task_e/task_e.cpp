#include <mpi.h>

#include <iostream>

// #include "../../common/BigNumber/src/bignumber.h"
#include "../../common/bigint.h"

int main(int argc, char *argv[])
{
    bigint ten = bigint("10");
    bigint power = bigint("10");
    bigint precision = big_pow(ten, power);

    bigint N = bigint("123");

    std::cout << precision << std::endl;
    std::cout << precision / N << std::endl;

    // int commsize, my_rank;
    // MPI_Init(&argc, &argv);
    // MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    // MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // MPI_Status status;
    // int buf = 1;

    // if (my_rank == 0)
    // {
    //     MPI_Send(&buf, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD);
    //     printf("Proc %d sent %d\n", my_rank, buf);
    // }

    // if (my_rank != 0)
    // {
    //     MPI_Recv(&buf, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD, &status);
    //     printf("Proc %d got %d\n", my_rank, buf);

    //     buf += 1,

    //     MPI_Send(&buf, 1, MPI_INT, (my_rank + 1) % commsize, 0, MPI_COMM_WORLD);
    //     printf("Proc %d sent %d\n", my_rank, buf);
    // }

    // if (my_rank == 0)
    // {
    //     MPI_Recv(&buf, 1, MPI_INT, commsize - 1, 0, MPI_COMM_WORLD, &status);
    //     printf("Proc %d got %d\n", my_rank, buf);
    // }

    // MPI_Finalize();
}