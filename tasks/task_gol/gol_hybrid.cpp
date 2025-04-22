#include "gol_hybrid.h"

GameOfLifeHybrid::GameOfLifeHybrid(size_t width, size_t height, int commsize, int my_rank, size_t threads_per_process) : GameOfLifeShared(width, get_worker_height(height, commsize, my_rank) + 2, threads_per_process)
{
    total_height = height;
    this->commsize = commsize;
    this->my_rank = my_rank;
    buf = new uint8_t[width];
}

int GameOfLifeHybrid::get_worker_height(size_t height, int commsize, int rank)
{
    return height / commsize + (rank < height % commsize);
}

GameOfLifeHybrid::~GameOfLifeHybrid()
{
    delete[] buf;
}

void GameOfLifeHybrid::send(size_t line, int destination) const
{
    for (int i = 0; i < width; i++)
    {
        buf[i] = field[line][i];
    }
    MPI_Send(buf, width, MPI_CHAR, destination, 0, MPI_COMM_WORLD);
}

void GameOfLifeHybrid::recv(size_t line, int source)
{
    MPI_Status status;
    MPI_Recv(buf, width, MPI_CHAR, source, 0, MPI_COMM_WORLD, &status);
    for (int i = 0; i < width; i++)
    {
        field[line][i] = buf[i];
    }
}

void GameOfLifeHybrid::sync()
{
    if (my_rank % 2 == 0)
    {
        send(1, (my_rank - 1 + commsize) % commsize);
        send(height - 2, (my_rank + 1 + commsize) % commsize);
        recv(height - 1, (my_rank + 1 + commsize) % commsize);
        recv(0, (my_rank - 1 + commsize) % commsize);
    }
    else
    {
        recv(height - 1, (my_rank + 1 + commsize) % commsize);
        recv(0, (my_rank - 1 + commsize) % commsize);
        send(1, (my_rank - 1 + commsize) % commsize);
        send(height - 2, (my_rank + 1 + commsize) % commsize);
    }
}

void GameOfLifeHybrid::set_cell(size_t x, size_t y, bool val)
{
    size_t prev_sum = 0;
    for (int i = 0; i < my_rank; i++)
    {
        prev_sum += get_worker_height(total_height, commsize, i);
    }
    if (prev_sum <= y && prev_sum + get_worker_height(total_height, commsize, my_rank) > y)
    {
        field[y - prev_sum + 1][x] = val;
    }
    sync();
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
    GameOfLife::fill_random(percentage);
    sync();
}

void GameOfLifeHybrid::print() const
{
    if (my_rank == 0)
    {
        for (size_t y = 1; y < height - 1; y++)
        {
            for (size_t x = 0; x < width; x++)
            {
                std::cout << (field[y][x] ? '#' : '.');
            }
            std::cout << "\n";
        }

        for (int i = 1; i < commsize; i++)
        {
            for (size_t y = 0; y < get_worker_height(total_height, commsize, i); y++)
            {
                MPI_Status status;
                MPI_Recv(buf, width, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
                for (size_t x = 0; x < width; x++)
                {
                    std::cout << (buf[x] ? '#' : '.');
                }
                std::cout << "\n";
            }
        }
        std::cout << std::endl;
    }
    else
    {
        for (size_t y = 1; y < height - 1; y++)
        {
            send(y, 0);
        }
    }
}
