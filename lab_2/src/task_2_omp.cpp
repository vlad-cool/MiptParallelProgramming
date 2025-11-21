#include <iostream>
#include <fstream>
#include <chrono>
#include <cmath>
#include <omp.h>

#define ISIZE 5000
#define JSIZE 5000

void some_loop(int j_0, double **a)
{
    for (int i = 0; i < ISIZE - 1; i++)
    {
        for (int j = j_0; j < JSIZE; j += 6)
        {
            a[i][j] = sin(0.2 * a[i + 1][j - 6]);
        }
    }
}

int main(int argc, char **argv)
{
    double **a = new double *[ISIZE];
    a[0] = new double[ISIZE * JSIZE];

    for (size_t i = 1; i < ISIZE; i++)
    {
        a[i] = a[i - 1] + JSIZE;
    }

    int i, j;

    std::ofstream ff;

    for (i = 0; i < ISIZE; i++)
    {
        for (j = 0; j < JSIZE; j++)
        {
            a[i][j] = 10 * i + j;
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
#pragma omp parallel sections
    {
#pragma omp section
        some_loop(6, a);
#pragma omp section
        some_loop(7, a);
#pragma omp section
        some_loop(8, a);
#pragma omp section
        some_loop(9, a);
#pragma omp section
        some_loop(10, a);
#pragma omp section
        some_loop(11, a);
    }
    //     for (i = 0; i < ISIZE - 1; i++)
    //     {
    // #pragma omp parallel sections
    //         {
    // #pragma omp section
    //             for (j = 6; j < JSIZE; j += 6)
    //             {
    //                 a[i][j] = sin(0.2 * a[i + 1][j - 6]);
    //             }
    // #pragma omp section
    //             for (j = 7; j < JSIZE; j += 6)
    //             {
    //                 a[i][j] = sin(0.2 * a[i + 1][j - 6]);
    //             }
    // #pragma omp section
    //             for (j = 8; j < JSIZE; j += 6)
    //             {
    //                 a[i][j] = sin(0.2 * a[i + 1][j - 6]);
    //             }
    // #pragma omp section
    //             for (j = 9; j < JSIZE; j += 6)
    //             {
    //                 a[i][j] = sin(0.2 * a[i + 1][j - 6]);
    //             }
    // #pragma omp section
    //             for (j = 10; j < JSIZE; j += 6)
    //             {
    //                 a[i][j] = sin(0.2 * a[i + 1][j - 6]);
    //             }
    // #pragma omp section
    //             for (j = 11; j < JSIZE; j += 6)
    //             {
    //                 a[i][j] = sin(0.2 * a[i + 1][j - 6]);
    //             }
    //         }
    //     }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << duration.count() / 1000000 << "." << std::setfill('0') << std::setw(6) << duration.count() % 1000000 << std::endl;

    if (argc > 1)
    {
        ff.open(argv[1]);
        for (i = 0; i < ISIZE; i++)
        {
            for (j = 0; j < JSIZE; j++)
            {
                ff << a[i][j];
            }
            ff << "\n";
        }
        ff.close();
    }

    delete[] a[0];
    delete[] a;
}