#include <iostream>
#include <sstream>
#include <omp.h>

int main(int argc, char *argv[])
{
#pragma omp parallel
    {
        std::stringstream s;

        s << "Hello, world!, from thread " << omp_get_thread_num() << " to other " << omp_get_num_threads() - 1 << " threads\n";
        std::cout << s.str();
    }
}