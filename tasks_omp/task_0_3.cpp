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
        while (active != omp_get_thread_num())
        {
        }

        var += 1;
        var *= 2;
        std::cout << omp_get_thread_num() << " " << var << std::endl;
        active++;
    }
}