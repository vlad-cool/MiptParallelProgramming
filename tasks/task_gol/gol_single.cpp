#include "gol_single.h"

uint8_t GameOfLife::count_neighbours(size_t x, size_t y)
{
    uint8_t res = 0;
    for (uint8_t i = 0; i <= 2; i++)
    {
        for (uint8_t j = 0; j <= 2; j++)
        {
            res += field[(y + height + j - 1) % height][(x + width + i - 1) % width];
        }
    }
    res -= field[y][x];
    return res;
}

GameOfLife::GameOfLife(size_t width, size_t height)
{
    this->width = width;
    this->height = height;
    field = std::vector<std::vector<bool>>(height, std::vector<bool>(width, false));
    next_field = std::vector<std::vector<bool>>(height, std::vector<bool>(width, false));
}

GameOfLife::~GameOfLife() {}

void GameOfLife::fill_random(uint32_t percentage)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 99);
    int random_num = distrib(gen);
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            field[y][x] = distrib(gen) < percentage;
        }
    }
}

void GameOfLife::set_cell(size_t x, size_t y, bool val)
{
    field[y][x] = val;
}

void GameOfLife::step(uint32_t steps)
{
    for (int32_t i = 0; i < steps; i++)
    {
        for (size_t y = 0; y < height; y++)
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
        std::swap(field, next_field);
    }
}

void GameOfLife::print() const
{
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            std::cout << (field[y][x] ? '#' : '.');
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}
