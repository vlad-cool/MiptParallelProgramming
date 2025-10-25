#include "gol_hybrid.h"

GameOfLifeHybrid::GameOfLifeHybrid(size_t width, size_t height, int commsize, int my_rank, bool optimize_send, size_t threads_per_process) : GameOfLife(width, get_worker_height(height, commsize, my_rank) + 2), GameOfLifeMpi(width, height, commsize, my_rank, optimize_send), GameOfLifeShared(width, get_worker_height(height, commsize, my_rank) + 2, threads_per_process)
{
}

void GameOfLifeHybrid::step(uint32_t steps)
{
    for (int32_t i = 0; i < steps; i++)
    {
        GameOfLifeShared::step();
        sync();
    }
}

std::ostream &operator<<(std::ostream &os, const GameOfLifeHybrid &p)
{
    p.print(os);
    return os;
}
