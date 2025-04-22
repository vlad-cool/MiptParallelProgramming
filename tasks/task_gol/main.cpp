#include <chrono>
#include <thread>

#include "gol_single.h"
#include "gol_shared.h"

int main()
{
    // GameOfLife gol(151, 14);
    GameOfLifeShared gol(151, 14, 3);
    gol.set_cell(3, 3, true);
    gol.set_cell(2, 4, true);
    gol.set_cell(2, 5, true);
    gol.set_cell(3, 5, true);
    gol.set_cell(4, 5, true);
    gol.fill_random(33);

    while (1)
    {
        gol.print();
        gol.step();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}