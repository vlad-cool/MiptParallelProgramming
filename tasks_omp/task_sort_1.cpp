#include <iostream>
#include <iomanip>
#include <string.h>
#include <cmath>
#include <random>
#include <stack>
#include <chrono>
#include <omp.h>
#include <algorithm>

// #define BUBBLE_THRESHOLD 16
#define BUBBLE_THRESHOLD 1

void quick_sort(int *array, size_t size)
{
    std::sort(array, array + size);
    // std::cerr << size << std::endl;
    if (size <= 1)
    {
        return;
    }

    size_t pivot = 0;

    for (size_t i = 1; i < size; i++)
    {
        if (array[i] < array[pivot])
        {
            std::swap(array[i], array[pivot]);
            pivot = i;
        }
    }

    quick_sort(array, pivot);
    quick_sort(array + pivot + 1, size - pivot - 1);
}

void sort(int *array, int *buf, size_t size, int depth_threshold, bool enable_qs)
{
    // if (size <= buble_threshold)
    // {
    //     for (size_t i = 0; i < size; i++)
    //     {
    //         for (size_t j = 1; j < i; j++)
    //         {
    //             if (array[i] > array[j])
    //             {
    //                 std::swap(array[i], array[j]);
    //             }
    //         }
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
        if (enable_qs)
        {
            quick_sort(array, size);
        }
        else
        {
            sort(array, buf, size_1, 0, enable_qs);
            sort(array + size_1, buf + size_1, size_2, 0, enable_qs);
        }
    }
    else
    {
#pragma omp task
        sort(array, buf, size_1, depth_threshold - 1, enable_qs);
#pragma omp task
        sort(array + size_1, buf + size_1, size_2, depth_threshold - 1, enable_qs);
#pragma omp taskwait
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

    bool enable_qs = false;

    if (argc > 3)
    {
        enable_qs = true;
    }

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

    sort(array, buf, size, depth_threshold, enable_qs);
    sort(array, buf, size, depth_threshold, enable_qs);
    sort(array, buf, size, depth_threshold, enable_qs);

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
