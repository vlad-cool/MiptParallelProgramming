#ifndef GOL_SHARED_H
#define GOL_SHARED_H

#include <cstdint>
#include <cstddef>
#include <vector>
#include <random>
#include <iostream>
#include <thread>
#include <atomic>

#include "gol_single.h"

class GameOfLifeShared : virtual public GameOfLife
{
protected:
    std::vector<std::thread> threads;
    std::atomic<bool> *thread_running;
    std::atomic<bool> running;
    void partial_step(size_t from, size_t to, size_t id);

public:
    GameOfLifeShared(size_t width, size_t height, size_t threads_number);
    ~GameOfLifeShared();
    virtual void step(uint32_t steps = 1) override;
    friend std::ostream &operator<<(std::ostream &os, const GameOfLifeShared &b);
};

#endif
