#include "gol_shared.h"

void GameOfLifeShared::partial_step(size_t from, size_t to, size_t id)
{
    while (running.load())
    {
        thread_running[id].wait(false);
        // unsigned int value = thread_counter.load();
        // std::cout << id << " " << value << std::endl;
        // while (value & (1 << id) == 0)
        // {
        //     thread_counter.wait(value);
        //     value = thread_counter.load();
        // }
        // std::cout << id << " " << value << std::endl;
        // std::cout << "Thread " << id << " started calculating" << std::endl;
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
        thread_running[id].store(false);
        thread_running[id].notify_all();
        // std::cout << "Thread " << id << " finished calculating" << std::endl;
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
    thread_running = new std::atomic<bool>(threads_number);
    size_t from = 0;
    for (size_t i = 0; i < threads_number; i++)
    {
        size_t step = height / threads_number + (i < (height % threads_number));
        threads.push_back(
            std::thread([from, step, i, this]()
                        { this->partial_step(from, from + step, i); }));
        thread_running[i].store(false);
        from += step;
    }
}

GameOfLifeShared::~GameOfLifeShared()
{
    running.store(false);
    for (size_t i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }
    delete[] thread_running;
}

void GameOfLifeShared::step(uint32_t steps)
{
    for (int32_t i = 0; i < steps; i++)
    {
        // std::cout << "Starting step" << std::endl;
        for (int i = 0; i < threads.size(); i++)
        {
            thread_running[i].store(true);
            thread_running[i].notify_one();
        }
        for (int i = 0; i < threads.size(); i++)
        {
            thread_running[i].wait(true);
        }
        std::swap(field, next_field);
    }
}

std::ostream &operator<<(std::ostream &os, const GameOfLifeShared &b)
{
    b.print(os);
    return os;
}
