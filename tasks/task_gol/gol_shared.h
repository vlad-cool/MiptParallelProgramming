#ifndef GOL_H_PTHREAD
#define GOL_H_PTHREAD

#include <cstdint>
#include <cstddef>
#include <vector>
#include <random>
#include <iostream>
#include <thread>
#include <atomic>

#include "gol_single.h"

class GameOfLifeShared : public GameOfLife
{
protected:
    std::vector<std::thread> threads;
    std::atomic<uint32_t> thread_counter;
    std::atomic<bool> running;
    void partial_step(size_t from, size_t to, size_t id);

public:
    GameOfLifeShared(size_t width, size_t height, size_t threads_number);
    ~GameOfLifeShared();
    void step(uint32_t steps = 1) override;
};

#endif
