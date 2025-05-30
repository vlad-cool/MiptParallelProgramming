#ifndef GOL_H
#define GOL_H

#include <cstdint>
#include <cstddef>
#include <vector>
#include <random>
#include <ostream>

class GameOfLife
{
protected:
    std::vector<std::vector<bool>> field, next_field;
    size_t width, height;
    uint8_t count_neighbours(size_t x, size_t y);

public:
    GameOfLife(size_t width, size_t height);
    ~GameOfLife();
    virtual void fill_random(uint32_t percentage);
    virtual void set_cell(size_t x, size_t y, bool val);
    virtual void step(uint32_t steps = 1);
    virtual void print(std::ostream &os) const;
    virtual void sync() {};
    friend std::ostream &operator<<(std::ostream &os, const GameOfLife &b);
};

#endif
