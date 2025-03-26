#include <mpi.h>
#include <stdio.h>

#include "../../common/BigNumber/src/bignumber.h"

int main(int argc, char *argv[])
{
    BigNumber b("5");   // BigNumber b is created with value 5.
    BigNumber c("-20"); // BigNumber c is created with value -20.
    BigNumber d("0");   // BigNumber d is created with value 0.
    BigNumber e = b;    // BigNumber e is created with value 5.
    BigNumber f = 30;   // BigNumber f is created with value 30.
    // BigNumber g = "2060";   //BigNumber g is created with value 2060.
    BigNumber h(22); // BigNumber h is created with value 22.

    std::cout << b << std::endl;

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