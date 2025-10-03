#include <iostream>
#include <iomanip>
#include <string.h>
#include <cmath>
#include <random>
#include <stack>
#include <chrono>
#include <omp.h>

// #define BUBBLE_THRESHOLD 16
#define BUBBLE_THRESHOLD 1

// void sort(int *array, int *buf, size_t size, int depth_threshold, size_t buble_threshold)
void sort(int *array, int *buf, size_t size, int depth_threshold)
{
    // if (size <= buble_threshold) {
    //     for (size_t i = 0; i < size; i++) {
    //         for (size_t k = 0; i + j < size; j++)
    //     }
    // }

    // std::cout << depth_threshold << std::endl;
    if (size <= 1)
    {
        return;
    }

    size_t size_1 = size / 2;
    size_t size_2 = size - size_1;

    if (depth_threshold == 0)
    {
#pragma omp task
        sort(array, buf, size_1, 0);
#pragma omp task
        sort(array + size_1, buf + size_1, size_2, 0);
#pragma omp taskwait
    }
    else
    {
        sort(array, buf, size_1, depth_threshold - 1);
        sort(array + size_1, buf + size_1, size_2, depth_threshold - 1);
    }

    size_t i_1 = 0, i_2 = size_1, i = 0;

    while (i_1 < size_1 && i_2 < size)
    {
        if (buf[i_1] <= buf[i_2])
        {
            array[i] = buf[i_1];
            i_1++;
        }
        else
        {
            array[i] = buf[i_2];
            i_2++;
        }
        i++;
    }
    while (i_1 < size_1)
    {
        array[i] = buf[i_1];
        i_1++;
        i++;
    }
    while (i_2 < size)
    {
        array[i] = buf[i_2];
        i_2++;
        i++;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: " << argv[1] << " N DEPTH_THRESHOLD" << std::endl;
        return 1;
    }

    size_t size = atoi(argv[1]);
    int depth_threshold = atoi(argv[2]);

    auto start = std::chrono::high_resolution_clock::now();

    int *array = new int[size];
    int *buf = new int[size];

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, size);

    for (size_t i = 0; i < size; i++)
    {
        array[i] = dist(gen);
    }

    sort(array, buf, size, depth_threshold);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    bool success = true;

    for (size_t i = 1; i < size; i++)
    {
        if (array[i - 1] > array[i])
        {
            success = false;
        }
    }

    std::cout << "Success: " << std::boolalpha << success;
    std::cout << ", sorted " << size << " numbers in " << duration.count() / 1000000 << "." << std::setfill('0') << std::setw(6) << duration.count() % 1000000 << " seconds" << std::endl;
}
