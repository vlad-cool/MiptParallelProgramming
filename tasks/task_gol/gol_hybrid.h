#ifndef GOL_HYBRID_H
#define GOL_HYBRID_H

#include <mpi.h>

#include "gol_mpi.h"
#include "gol_shared.h"

class GameOfLifeHybrid : virtual public GameOfLifeMpi, public GameOfLifeShared
{
public:
    GameOfLifeHybrid(size_t width, size_t height, int commsize, int my_rank, size_t executors_per_process);
    virtual void set_cell(size_t x, size_t y, bool val) override;
    void step(uint32_t steps = 1) override;
    void print() const override;
    void fill_random(uint32_t percentage) override;
};

#endif
