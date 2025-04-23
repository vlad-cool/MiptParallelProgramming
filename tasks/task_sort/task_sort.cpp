#include <iostream>
#include <iomanip>
#include <string.h>
#include <cmath>
#include <random>
#include <stack>
#include <mpi.h>

#define SPLIT_NUMBER 2
#define BUBBLE_THRESHOLD 16

struct SortTask {
    int executior_rangs[SPLIT_NUMBER];
    int size;
};

int commsize, my_rank;
std::stack<SortTask> sort_tasks;

void swap(int *a, int *b)
{
    int c = *a;
    *a = *b;
    *b = c;
}

void sort(int *array, size_t size)
{
    if (size < BUBBLE_THRESHOLD)
    {
        bool sorted = false;
        ssize_t distance = size;

        while (!sorted || distance > 1)
        {
            distance = distance * 2 / 3;
            if (distance < 1)
            {
                distance = 1;
            }

            sorted = true;
            for (ssize_t i = 0; i + distance < size; i++)
            {
                if (array[i] > array[i + distance])
                {
                    sorted = false;
                    swap(array + i, array + i + distance);
                }
            }
        }
    }
    else
    {
        SortTask sort_task;
        int part_size = (size + SPLIT_NUMBER - 1) / SPLIT_NUMBER;
        for (int i = 0; i < SPLIT_NUMBER; i++)
        {
            if (i == SPLIT_NUMBER - 1)
            {
                part_size = size - part_size * (SPLIT_NUMBER - 1);
            }
            sort_task.executior_rangs[i] = (my_rank + 1 + i) % commsize;
            MPI_Send(&part_size, 1, MPI_INT, sort_task.executior_rangs[i], 0, MPI_COMM_WORLD);
            MPI_Send(array + part_size * i, part_size, MPI_INT, sort_task.executior_rangs[i], 0, MPI_COMM_WORLD);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "No numbers number provided" << std::endl;
        return 1;
    }
    long long numbers = atoi(argv[1]);

    int commsize, my_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // if (commsize == 1)
    // {
    //     std::cout << "Not enough threads, minimum two required"
    // }

    
    if (my_rank == 0)
    {
        int *array = new int[numbers];
        std::random_device rd;  
        std::mt19937 gen(rd()); 
        std::uniform_int_distribution<int> dist(0, numbers); 

        int random_num = dist(gen); 

        for (long long i = 0; i < numbers; i++)
        {
            array[i] = dist(gen);
            std::cout << array[i] << std::endl;
        }
        sort(array, numbers);
    }

    

    // MPI_Status status;

    // long long from, to;

    // from = N / commsize * my_rank;
    // if (my_rank + 1 == commsize)
    //     to = N;
    // else
    //     to = N / commsize * (my_rank + 1);

    // BigInt inverted_factorial = precision;
    // BigInt res = 0;

    // for (unsigned int i = from; i < to; i++)
    // {
    //     if (i != 0)
    //     {
    //         inverted_factorial = inverted_factorial / i;
    //     }
    //     res = res + inverted_factorial;
    // }

    // if (my_rank + 1 != commsize)
    // {
    //     int buf_size;
    //     MPI_Recv(&buf_size, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD, &status);
    //     unsigned int *buf = new unsigned int[buf_size];
    //     MPI_Recv(buf, buf_size, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD, &status);

    //     BigInt next_res(buf, buf_size);
    //     delete[] buf;
    //     res = (next_res * inverted_factorial).divide_power_10(power) + res;
    // }

    // if (my_rank != 0)
    // {
    //     int buf_size = res.get_size();
    //     MPI_Send(&buf_size, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD);
    //     unsigned int *buf = res.get_data();
    //     MPI_Send(buf, buf_size, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD);
    // }

    // if (my_rank == 0)
    // {
    //     std::cout << res.divide_power_10(EXTRA_DGITS) << std::endl;
    // }

    delete[] array;
    
    MPI_Finalize();
}
