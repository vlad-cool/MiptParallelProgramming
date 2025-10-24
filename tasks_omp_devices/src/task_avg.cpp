#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>
#include <cmath>
#include <omp.h>

float avg_dev(float *a, size_t size)
{
    float sum = 0.0;
#pragma omp target map(to : a[ : size]) map(tofrom : sum)
#pragma omp teams distribute parallel for reduction(+ : sum)
    for (size_t i = 0; i < size; i++)
        sum += a[i];
    return sum / size;
}

float avg(float *a, size_t size)
{
    float sum = 0.0;

#pragma omp parallel for reduction(+ : sum)
    for (size_t i = 0; i < size; i++)
        sum += a[i];
    return sum / size;
}

int main()
{
    std::cout << "Num devices: " << omp_get_num_devices() << std::endl;
    for (int i = 0; i < omp_get_num_devices(); i++)
    {
        std::cout << omp_get_uid_from_device(i) << std::endl;
    }

    size_t size = 1024 * 1024 * 256;

    float *a = new float[size];

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> dist(10, 20);

    for (size_t i = 0; i < size; i++)
    {
        a[i] = dist(gen);
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        float res = avg_dev(a, size);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cerr << "Computer average of " << size << " floats on gpu in " << duration.count() / 1000000 << "." << std::setfill('0') << std::setw(6) << duration.count() % 1000000 << " seconds, result is " << res << std::endl;
    }
    {

        auto start = std::chrono::high_resolution_clock::now();
        float res = avg(a, size);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cerr << "Computer average of " << size << " floats on cpu in " << duration.count() / 1000000 << "." << std::setfill('0') << std::setw(6) << duration.count() % 1000000 << " seconds, result is " << res << std::endl;
    }
}
