#include <iostream>
#include <sstream>
#include <vector>
#include <omp.h>

int main()
{
    omp_set_num_threads(4);

    std::cout << "default" << std::endl;
    std::cout << "thread_num, i" << std::endl;
#pragma omp parallel for
    for (int i = 0; i < 65; i++)
    {
        std::stringstream s;

        s << omp_get_thread_num() << ", " << i << std::endl;

        std::cout << s.str();
    }

    std::cout << "static chunk 1" << std::endl;
    std::cout << "thread_num, i" << std::endl;
#pragma omp parallel for schedule(static, 1)
    for (int i = 0; i < 65; i++)
    {
        std::stringstream s;

        s << omp_get_thread_num() << ", " << i << std::endl;

        std::cout << s.str();
    }

    std::cout << "static chunk 4" << std::endl;
    std::cout << "thread_num, i" << std::endl;
#pragma omp parallel for schedule(static, 4)
    for (int i = 0; i < 65; i++)
    {
        std::stringstream s;

        s << omp_get_thread_num() << ", " << i << std::endl;

        std::cout << s.str();
    }

    std::cout << "dynamic chunk 1" << std::endl;
    std::cout << "thread_num, i" << std::endl;
#pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < 65; i++)
    {
        std::stringstream s;

        s << omp_get_thread_num() << ", " << i << std::endl;

        std::cout << s.str();
    }

    std::cout << "dynamic chunk 4" << std::endl;
    std::cout << "thread_num, i" << std::endl;
#pragma omp parallel for schedule(dynamic, 4)
    for (int i = 0; i < 65; i++)
    {
        std::stringstream s;

        s << omp_get_thread_num() << ", " << i << std::endl;

        std::cout << s.str();
    }

    std::cout << "guided chunk 1" << std::endl;
    std::cout << "thread_num, i" << std::endl;
#pragma omp parallel for schedule(guided, 1)
    for (int i = 0; i < 65; i++)
    {
        std::stringstream s;

        s << omp_get_thread_num() << ", " << i << std::endl;

        std::cout << s.str();
    }

    std::cout << "guided chunk 4" << std::endl;
    std::cout << "thread_num, i" << std::endl;
#pragma omp parallel for schedule(guided, 4)
    for (int i = 0; i < 65; i++)
    {
        std::stringstream s;

        s << omp_get_thread_num() << ", " << i << std::endl;

        std::cout << s.str();
    }
}