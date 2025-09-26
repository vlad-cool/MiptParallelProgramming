#include <iostream>
#include <sstream>
#include <omp.h>

void print_info()
{
    std::stringstream s;
    for (int i = 0; i <= omp_get_level(); i++)
    {
        s << " -> " << omp_get_ancestor_thread_num(i);
    }

    s << " thread_num: " << omp_get_thread_num();
    s << " num_threads: " << omp_get_num_threads();
    s << std::endl;

    std::cout << s.str();
}

int main(int argc, char *argv[])
{
    // omp_set_num_threads(3);
    omp_set_max_active_levels(5);

#pragma omp parallel num_threads(3)
    {
        print_info();

#pragma omp parallel num_threads(2)
        {
            print_info();

#pragma omp parallel num_threads(3)
            {
                print_info();
            }
        }
    }
}