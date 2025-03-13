#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("No arguments provided, exiting");
        return -1;
    }
    if (argc > 2)
    {
        printf("Too many arguments provided, exiting");
        return -1;
    }

    int n = atoi(argv[1]);

    int commsize, my_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    double sum = 0;
    int to_count = (n + commsize - 1) / commsize;

    for (int i = 0; i < to_count; i++)
    {
        int j = to_count * my_rank + i + 1;

        if (j > n)
            break;

        sum += (double)1 / (j);

        printf("i = %d\n", j);
    }

    printf("Hello, world!, rank: %d, total executor count: %d\n", my_rank, commsize);

    if (my_rank != 0)
    {
        MPI_Send(&sum, 1, MPI_2DOUBLE_PRECISION, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        for (int i = 1; i < commsize; i++)
        {
            double buf;
            MPI_Status status;
            MPI_Recv(&buf, 1, MPI_2DOUBLE_PRECISION, i, 0, MPI_COMM_WORLD, &status);
            sum += buf;
        }
        printf("Total sum: %f\n", sum);
    }


    MPI_Finalize();
}