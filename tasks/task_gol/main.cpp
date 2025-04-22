#include <chrono>
#include <thread>

#include "gol_single.h"
#include "gol_shared.h"
#include "gol_mpi.h"

int main(int argc, char *argv[])
{
    // GameOfLife gol(151, 14);
    // GameOfLifeShared gol(151, 14, 3);

    int commsize, my_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    GameOfLifeMpi gol(151, 14, commsize, my_rank);

    gol.set_cell(3, 3, true);
    gol.set_cell(2, 4, true);
    gol.set_cell(2, 5, true);
    gol.set_cell(3, 5, true);
    gol.set_cell(4, 5, true);

    gol.set_cell(42 + 0, 0, true);
    gol.set_cell(42 + 1, 1, true);
    gol.set_cell(42 + 2, 2, true);
    gol.set_cell(42 + 3, 3, true);
    gol.set_cell(42 + 4, 4, true);
    gol.set_cell(42 + 5, 5, true);
    gol.set_cell(42 + 6, 6, true);
    gol.set_cell(42 + 7, 7, true);
    gol.set_cell(42 + 8, 8, true);
    gol.set_cell(42 + 9, 9, true);
    gol.set_cell(42 + 10, 10, true);
    gol.set_cell(42 + 11, 11, true);
    gol.set_cell(42 + 12, 12, true);
    gol.set_cell(42 + 13, 13, true);

    // gol.fill_random(33);

    while (1)
    {
        gol.print();
        gol.step();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    MPI_Finalize();
}
