#include <iostream>
#include <random>
#include <omp.h>

// int main()
// {
//     std::cout << omp_get_num_devices() << std::endl;

//     size_t size = 1024 * 1024 * 256;

//     int *a = new int[size];
//     int *b = new int[size];

//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::normal_distribution<int> dist(0.0, 1.0);

//     for (size_t i = 0; i < size; i++)
//     {
//         a[i] = dist(gen);
//     }
// }

int disp(int a[], size_t size, size_t teams)
{
    int avg_1 = 0, avg_2 = 0;
#pragma omp target map(to : a[ : size])
#pragma omp teams num_teams(teams)
    {
        size_t chunk_size = (size + teams - 1) / teams;
        size_t chunk_start = chunk_size * omp_get_team_num();
        if (omp_get_team_num() == teams - 1)
        {
            chunk_size = size - chunk_start;
        }
        size_t chunk_end = chunk_start + chunk_size;

        // printf("Running on device with %d teams in total and %d threads in each team\n", omp_get_num_teams(), omp_get_num_threads());

        // std::cout << size << " " << chunk_start << " " << chunk_end << std::endl;
        // printf("%zu %zu %zu\n", size, chunk_start, chunk_end);

#pragma omp parallel for reduction(+ : avg_2)
        // #pragma omp parallel for reduction(+ : avg_1, avg_2)
        for (size_t i = chunk_start; i < chunk_end; i++)
        {
            // avg_1 += a[i];
            avg_2 += a[i] * a[i];
        }
        // printf("   %lf %lf\n", avg_1, avg_2);
    }

    return (avg_2 / size) - (avg_1 / size) * (avg_1 / size);
}

int main()
{
    std::cout << "Num devices: " << omp_get_num_devices() << std::endl;
    size_t size = 1024 * 1024 * 256;
    int *a = new int[size];

    std::random_device rd;
    std::mt19937 gen(rd());
    // std::normal_distribution<int> dist(0.0, 1.0);
    std::uniform_int_distribution<int> dist(0, 200);

    for (size_t i = 0; i < size; i++)
    {
        a[i] = dist(gen);
    }

#pragma omp target
    {
        if (omp_is_initial_device())
        {
            printf("Running on host\n");
        }
        else
        {
            // int nteams = omp_get_num_teams();
            // int nthreads = omp_get_num_threads();
            // printf("Running on device with %d teams in total and %d threads in each team\n", nteams, nthreads);

            // int *b = new int[size];

            std::cout << disp(a, size, 10) << std::endl;
        }
    }
}

// int main()
// {
//     std::cout << omp_get_num_devices() << std::endl;

//     size_t size = 1024 * 1024 * 256;

//     int *a = new int[size];
//     int *b = new int[size];

//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::normal_distribution<int> dist(0.0, 1.0);

//     for (size_t i = 0; i < size; i++)
//     {
//         a[i] = dist(gen);
//     }

//     std::cout << disp(a, size, 8) << std::endl;
// }
