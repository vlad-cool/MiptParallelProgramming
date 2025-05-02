#include <iostream>
#include <cassert>
#include <chrono>
#include <iomanip>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int n = 100;
    int buf_size = 1000;
    int size, rank;
    
    int *buf = new int[buf_size];
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    assert(size == 2 && "Expected running on exactly two processes");
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++)
    {
        if (rank == 0)
        {
            MPI_Send(buf, buf_size, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(buf, buf_size, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (rank == 1)
        {
            MPI_Recv(buf, buf_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(buf, buf_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    duration /= 2;
    duration /= n;
    if (rank == 0)
    {
        std::cout << duration / 1000000000 << "." << std::setfill('0') << std::setw(9) << duration % 1000000000 << " seconds" << std::endl;
    }

    MPI_Finalize();

    delete[] buf;
}
