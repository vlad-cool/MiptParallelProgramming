#include "gol_hybrid.h"

GameOfLifeHybrid::GameOfLifeHybrid(size_t width, size_t height, int commsize, int my_rank, size_t executors_per_process) : GameOfLifeMpi(width, height, commsize, my_rank), GameOfLifeShared(width, height, executors_per_process)
{
}

void GameOfLifeHybrid::set_cell(size_t x, size_t y, bool val)
{
    GameOfLifeMpi::set_cell(x, y, val);
}

void GameOfLifeHybrid::step(uint32_t steps)
{
    for (int32_t i = 0; i < steps; i++)
    {
        GameOfLifeShared::step();
        sync();
    }
}

void GameOfLifeHybrid::fill_random(uint32_t percentage)
{
    GameOfLifeMpi::fill_random(percentage);
}

void GameOfLifeHybrid::print() const
{
    GameOfLifeMpi::print();
}

