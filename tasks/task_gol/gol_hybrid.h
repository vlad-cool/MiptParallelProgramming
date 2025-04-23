#ifndef GOL_HYBRID_H
#define GOL_HYBRID_H

#include "gol_shared.h"
#include "gol_mpi.h"

class GameOfLifeHybrid : public GameOfLifeShared, public GameOfLifeMpi
{
public:
    GameOfLifeHybrid(size_t width, size_t height, int commsize, int my_rank, size_t threads_per_process);
    virtual void step(uint32_t steps = 1) override;
    friend std::ostream &operator<<(std::ostream &os, const GameOfLifeHybrid &b);
};

#endif
