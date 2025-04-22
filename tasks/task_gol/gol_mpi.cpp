#include "gol_mpi.h"

GameOfLifeMpi::GameOfLifeMpi(size_t width, size_t height, int commsize, int my_rank) : GameOfLife(width, get_worker_height(height, commsize, my_rank) + 2)
{
    total_height = height;
    this->commsize = commsize;
    this->my_rank = my_rank;
    buf = new uint8_t[width];
}

int GameOfLifeMpi::get_worker_height(size_t height, int commsize, int rank)
{
    return height / commsize + (rank < height % commsize);
}

GameOfLifeMpi::~GameOfLifeMpi()
{
    delete[] buf;
}

void GameOfLifeMpi::send(size_t line, int destination) const
{
    for (int i = 0; i < width; i++)
    {
        buf[i] = field[line][i];
    }
    MPI_Send(buf, width, MPI_CHAR, destination, 0, MPI_COMM_WORLD);
}

void GameOfLifeMpi::recv(size_t line, int source)
{
    MPI_Status status;
    MPI_Recv(buf, width, MPI_CHAR, source, 0, MPI_COMM_WORLD, &status);
    for (int i = 0; i < width; i++)
    {
        field[line][i] = buf[i];
    }
}

void GameOfLifeMpi::sync()
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

void GameOfLifeMpi::set_cell(size_t x, size_t y, bool val)
{
    size_t prev_sum = 0;
    for (int i = 0; i < my_rank; i++)
    {
        prev_sum += get_worker_height(total_height, commsize, i);
    }
    if (prev_sum <= y && prev_sum + get_worker_height(total_height, commsize, my_rank) > y)
    {
        field[y - prev_sum][x] = val;
    }
    sync();
}

void GameOfLifeMpi::step(uint32_t steps)
{
    for (int32_t i = 0; i < steps; i++)
    {
        GameOfLife::step();
        sync();
    }
}

void GameOfLifeMpi::fill_random(uint32_t percentage)
{
    GameOfLife::fill_random(percentage);
    sync();
}

void GameOfLifeMpi::print() const
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
