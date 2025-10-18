#include <iostream>
#include <sstream>
#include <vector>
#include <omp.h>

int main(int argc, char *argv[])
{
    int var = 0;
    volatile int active = 0;
#pragma omp parallel
    {
        int num_thread = omp_get_thread_num();
        while (active != num_thread)
        {
        }

        var += 1;
        var *= 2;
        std::cout << num_thread << " " << var << std::endl;
        active++;
    }
}