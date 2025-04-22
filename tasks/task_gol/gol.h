#ifndef GOL_H
#define GOL_H

#include <cstdint>
#include <cstddef>
#include <vector>
#include <random>
#include <iostream>

class GameOfLife
{
protected:
    std::vector<std::vector<bool>> field, next_field;
    size_t width, height;
    uint8_t count_neighbours(size_t x, size_t y);
public:
    GameOfLife(size_t width, size_t height);
    ~GameOfLife();
    void fill_random(uint32_t percentage);
    void set_cell(size_t x, size_t y, bool val);
    virtual void step(uint32_t steps = 1);
    virtual void print() const;
};

#endif
