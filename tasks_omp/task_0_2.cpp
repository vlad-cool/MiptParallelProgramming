#include <iostream>
#include <sstream>
#include <vector>
#include <omp.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " N" << std::endl;
        return 1;
    }

    int N = std::stoi(argv[1]);

    int thread_num = omp_get_num_threads();
    std::vector<double> res_vec(thread_num);
    
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int to_count = (N + thread_num - 1) / thread_num;

        for (int i = to_count * thread_id; i < N && i < to_count * (thread_id + 1); i++)
        {
            res_vec[thread_id] += 1.0 / (i + 1);
        }
    }

    double res = 0;

    for (auto i = res_vec.begin(); i != res_vec.end(); i++)
    {
        res += *i;
    }

    std::cout << res << std::endl;
}