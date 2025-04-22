#ifndef GOL_MPI_H
#define GOL_MPI_H

#include <mpi.h>

#include "gol_single.h"

class GameOfLifeMpi : public GameOfLife
{
protected:
    size_t total_height;
    int commsize;
    int my_rank;
    mutable uint8_t *buf;
    void send(size_t line, int destination) const;
    void recv(size_t line, int source);
    void sync();
    static int get_worker_height(size_t height, int commsize, int rank);

public:
    GameOfLifeMpi(size_t width, size_t height, int commsize, int my_rank);
    ~GameOfLifeMpi();
    virtual void set_cell(size_t x, size_t y, bool val) override;
    void step(uint32_t steps = 1) override;
    void print() const override;
    void fill_random(uint32_t percentage) override;
};

#endif
