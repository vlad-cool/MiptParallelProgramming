#include <iostream>
#include <random>
#include <omp.h>

float dotprod(float B[], float C[], int N)
{
    float sum0 = 0.0, sum1 = 0.0;
#pragma omp target map(to : B[ : N], C[ : N]) map(tofrom : sum0)
    // #pragma omp teams num_teams(2)
    {
        if (omp_is_initial_device())
        {
            printf("Running on host\n");
        }

#pragma omp parallel for reduction(+ : sum0)
        for (int i = 0; i < N / 2; i++)
            sum0 += B[i] * C[i];
    }
    return sum0 + sum1;
}

int main()
{
    std::cout << "Num devices: " << omp_get_num_devices() << std::endl;
    std::cout << omp_get_uid_from_device(0) << std::endl;

    size_t size = 1024 * 1024 * 128;

    float *a = new float[size];
    float *b = new float[size];

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> dist(10, 20);

    for (size_t i = 0; i < size; i++)
    {
        a[i] = dist(gen);
        b[i] = dist(gen);
    }

    std::cout << dotprod(a, b, size) << std::endl;
}
