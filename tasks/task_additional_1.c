#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int commsize, my_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // void *buffer = malloc(sizeof(int) * 200000000);
    // MPI_Buffer_attach(buffer, sizeof(int) * 200000000);

    if (argc != 3)
    {
        printf("Wrong number of arguments, exiting\n");
        return -1;
    }

    int send_type = atoi(argv[1]);
    int buf_size = atoi(argv[2]);

    if (my_rank == 0)
    {
        int *buf = malloc(buf_size * sizeof(int));
        struct timespec t_start, t_end;

        printf("Sending message of size %5d using ", buf_size);

        clock_gettime(CLOCK_MONOTONIC, &t_start);

        switch (send_type)
        {
        default:
            printf("Unknown type of sender, using MPI_SEND(...)\b");
        case 0:
            printf("MPI_Send ");
            MPI_Send(buf, buf_size, MPI_INT, 1, 0, MPI_COMM_WORLD);
            break;
        case 1:
            MPI_Ssend(buf, buf_size, MPI_INT, 1, 0, MPI_COMM_WORLD);
            printf("MPI_Ssend");
            break;
        case 2:
            printf("MPI_Rsend");
            MPI_Rsend(buf, buf_size, MPI_INT, 1, 0, MPI_COMM_WORLD);
            break;
        case 3:
            printf("MPI_Bsend");
            MPI_Bsend(buf, buf_size, MPI_INT, 1, 0, MPI_COMM_WORLD);
            break;
        }
        clock_gettime(CLOCK_MONOTONIC, &t_end);

        printf(" took %f seconds\n", (t_end.tv_sec - t_start.tv_sec) + (double)(t_end.tv_nsec - t_start.tv_nsec) / 1000000000);
    }
    else if (my_rank == 1)
    {
        int *buf = malloc(buf_size * sizeof(int));
        MPI_Status status;
        sleep(2);
        MPI_Recv(buf, buf_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    MPI_Finalize();
}