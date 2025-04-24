#include "gol_mpi.h"

GameOfLifeMpi::GameOfLifeMpi(size_t width, size_t height, int commsize, int my_rank) : GameOfLife(width, get_worker_height(height, commsize, my_rank) + 2)
{
    total_height = height;
    this->commsize = commsize;
    this->my_rank = my_rank;
    line = std::vector<bool>(width, false);
    buf = new uint8_t[width / sizeof(uint8_t) + 1];
}

int GameOfLifeMpi::get_worker_height(size_t height, int commsize, int rank)
{
    return height / commsize + (rank < height % commsize);
}

GameOfLifeMpi::~GameOfLifeMpi()
{
    delete[] buf;
}

void GameOfLifeMpi::send(const std::vector<bool> &line, int destination) const
{
    for (int i = 0; i < width / sizeof(uint8_t) + 1; i++)
    {
        buf[i] = 0;
    }
    for (int i = 0; i < width; i++)
    {
        buf[i / sizeof(uint8_t)] |= line[i] << (i % sizeof(uint8_t));
    }
    MPI_Send(buf, width / sizeof(uint8_t) + 1, MPI_CHAR, destination, 0, MPI_COMM_WORLD);
}

void GameOfLifeMpi::recv(std::vector<bool> &line, int source) const
{
    MPI_Status status;
    MPI_Recv(buf, width / sizeof(uint8_t) + 1, MPI_CHAR, source, 0, MPI_COMM_WORLD, &status);
    for (int i = 0; i < width; i++)
    {
        line[i] = buf[i / sizeof(uint8_t)] >> (i % sizeof(uint8_t)) & 1;
    }
}

void GameOfLifeMpi::sync()
{
    if (commsize == 1)
    {
        return;
    }
    if (my_rank % 2 == 0)
    {
        send(field[1], (my_rank - 1 + commsize) % commsize);
        send(field[height - 2], (my_rank + 1 + commsize) % commsize);
        recv(field[height - 1], (my_rank + 1 + commsize) % commsize);
        recv(field[0], (my_rank - 1 + commsize) % commsize);
    }
    else
    {
        recv(field[height - 1], (my_rank + 1 + commsize) % commsize);
        recv(field[0], (my_rank - 1 + commsize) % commsize);
        send(field[1], (my_rank - 1 + commsize) % commsize);
        send(field[height - 2], (my_rank + 1 + commsize) % commsize);
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
        field[y - prev_sum + 1][x] = val;
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

void GameOfLifeMpi::print(std::ostream &os) const
{
    if (my_rank == 0)
    {
        for (size_t y = 1; y < height - 1; y++)
        {
            for (size_t x = 0; x < width; x++)
            {
                os << (field[y][x] ? '#' : '.');
            }
            os << "\n";
        }

        for (int i = 1; i < commsize; i++)
        {
            for (size_t y = 0; y < get_worker_height(total_height, commsize, i); y++)
            {
                recv(line, i);
                for (size_t x = 0; x < width; x++)
                {
                    os << (line[x] ? '#' : '.');
                }
                os << "\n";
            }
        }
    }
    else
    {
        for (size_t y = 1; y < height - 1; y++)
        {
            send(field[y], 0);
        }
    }
}

std::ostream &operator<<(std::ostream &os, const GameOfLifeMpi &b)
{
    b.print(os);
    return os;
}
