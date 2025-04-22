#include "gol_shared.h"

void GameOfLifeShared::partial_step(size_t from, size_t to, size_t id)
{
    while (running)
    {
        for (size_t y = from; y < to; y++)
        {
            for (size_t x = 0; x < width; x++)
            {
                uint8_t neighbours = count_neighbours(x, y);
                if (field[y][x])
                {
                    next_field[y][x] = neighbours == 2 || neighbours == 3;
                }
                else
                {
                    next_field[y][x] = neighbours == 3;
                }
            }
        }
        thread_counter.fetch_xor(1 << id);
    }
}

GameOfLifeShared::GameOfLifeShared(size_t width, size_t height, size_t threads_number) : GameOfLife(width, height)
{
    running = true;
    this->width = width;
    this->height = height;
    field = std::vector<std::vector<bool>>(height, std::vector<bool>(width, false));
    next_field = std::vector<std::vector<bool>>(height, std::vector<bool>(width, false));
    threads.reserve(threads_number);
    size_t from = 0;
    for (size_t i = 0; i < threads_number; i++)
    {
        size_t step = height / threads_number + (i < (height % threads_number));
        threads.push_back(
            std::thread([from, step, i, this]()
                        { this->partial_step(from, from + step, i); }));

        std::cout
            << height << " " << from << " " << from + step << std::endl;
        from += step;
    }
}

GameOfLifeShared::~GameOfLifeShared()
{
    running = false;
    for (size_t i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }
}

void GameOfLifeShared::step(uint32_t steps)
{
    for (int32_t i = 0; i < steps; i++)
    {
        thread_counter.store((1 << threads.size()) - 1);
        while (thread_counter.load() != 0)
        {
        }
        std::swap(field, next_field);
    }
}
