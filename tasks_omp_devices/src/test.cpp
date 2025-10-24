#include <iostream>
#include <random>
#include <omp.h>

float disp(float a[], size_t size, size_t teams)
{
    // float avg_1 = 0, avg_2 = 0;

    // omp_set_num_threads(256);

    // #pragma omp target map(to : a[0 : size]) map(from : result)
    // #pragma omp teams num_teams(256)
    float avg_1 = 0;

#pragma omp teams num_teams(256) reduction(+ : avg_1)
    {
        size_t chunk_size = (size + teams - 1) / teams;
        size_t chunk_start = chunk_size * omp_get_team_num();
        if (omp_get_team_num() == teams - 1)
            chunk_size = size - chunk_start;

        size_t chunk_end = chunk_start + chunk_size;

#pragma omp parallel for reduction(+ : avg_1)
        for (size_t i = chunk_start; i < chunk_end; i++)
        {
            // avg_1 += a[i];
            // avg_1 += a[i] * a[i];
            avg_1 += 1;
        }
    }

    // return (avg_2 / size) - (avg_1 / size) * (avg_1 / size);
    return avg_1;
}

int main()
{
    std::cout << "Num devices: " << omp_get_num_devices() << std::endl;

    size_t size = 1024 * 1024 * 16; // ~16 MB
    float *a = new float[size];

    std::random_device rd;
    std::mt19937 gen(rd());
    // std::uniform_float_distribution<float> dist(0, 200);

    std::normal_distribution<float> dist(10, 20);

    for (size_t i = 0; i < size; i++)
        a[i] = dist(gen);

    float result;
#pragma omp target map(to : a[0 : size]) map(from : result)
    {
        if (omp_is_initial_device())
            printf("Running on host\n");
        else
        {
            int nteams = omp_get_num_teams();
            int nthreads = omp_get_num_threads();
            // #pragma omp teams num_teams(256)
            result = disp(a, size, 8);
            printf("Running on device with %d teams and %d threads\n", nteams, nthreads);
        }
    }

    std::cout << "Result: " << result << std::endl;
    delete[] a;
}
