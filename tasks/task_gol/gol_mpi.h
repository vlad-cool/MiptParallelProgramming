#ifndef GOL_MPI_H
#define GOL_MPI_H

#include <mpi.h>

#include "gol_single.h"

class GameOfLifeMpi : virtual public GameOfLife
{
protected:
    size_t total_height;
    int commsize;
    int my_rank;
    mutable uint8_t *buf;
    void send(const std::vector<bool> &line, int destination) const;
    void recv(std::vector<bool> &line, int source) const;
    void sync();
    static int get_worker_height(size_t height, int commsize, int rank);
    mutable std::vector<bool> line;

public:
    GameOfLifeMpi(size_t width, size_t height, int commsize, int my_rank);
    ~GameOfLifeMpi();
    virtual void set_cell(size_t x, size_t y, bool val) override;
    virtual void step(uint32_t steps = 1) override;
    virtual void fill_random(uint32_t percentage) override;
    virtual void print(std::ostream &os) const override;
    friend std::ostream &operator<<(std::ostream &os, const GameOfLifeMpi &b);
};

#endif
