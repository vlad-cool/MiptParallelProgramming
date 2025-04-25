#include <iostream>
#include <iomanip>
#include <string.h>
#include <cmath>
#include <random>
#include <stack>
#include <chrono>
#include <mpi.h>

#define BUBBLE_THRESHOLD 16

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
        size_t pivot = 0;
        swap(array, array + pivot);
        for (int i = 1; i < size; i++)
        {
            if (array[i] < array[pivot])
            {
                swap(array + i, array + pivot + 1);
                swap(array + pivot, array + pivot + 1);
                pivot++;
            }
        }
        sort(array, pivot);
        sort(array + pivot + 1, size - pivot - 1);
    }
}

int get_size(int size, int rank, int commsize)
{
    return (size / commsize) + (rank < size % commsize);
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

    if (my_rank == 0)
    { 
        auto start = std::chrono::high_resolution_clock::now();
        
        int *array = new int[numbers];
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, numbers);
        int random_num = dist(gen);

        // for (long long i = 0; i < numbers; i++)
        // {
        //     array[i] = dist(gen);
        //     std::cout << array[i] << " ";
        // }
        // std::cout << std::endl;
        
        int offset = get_size(numbers, 0, commsize);
        for (int i = 1; i < commsize; i++)
        {
            int step = get_size(numbers, i, commsize);
            MPI_Send(&step, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(array + offset, step, MPI_INT, i, 0, MPI_COMM_WORLD);
            offset += step;
        }
        
        sort(array, get_size(numbers, 0, commsize));
        
        int **recieve_buffers = new int *[commsize];
        for (int i = 0; i < commsize; i++)
        {
            recieve_buffers[i] = new int[get_size(numbers, i, commsize)];
        }
        for (int i = 1; i < commsize; i++)
        {
            MPI_Status status;
            MPI_Recv(recieve_buffers[i], get_size(numbers, i, commsize), MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        }
        for (size_t i = 0; i < get_size(numbers, 0, commsize); i++)
        {
            recieve_buffers[0][i] = array[i];
        }
        int *indexes = new int[commsize];
        for (int i = 0; i < commsize; i++)
        {
            indexes[i] = 0;
        }
        for (int i = 0; i < numbers; i++)
        {
            int min_index = -1;
            int min = 0;
            for (int j = 0; j < commsize; j++)
            {
                if (indexes[j] == get_size(numbers, j, commsize))
                {
                    continue;
                }
                if (min_index == -1 || recieve_buffers[j][indexes[j]] < min)
                {
                    min_index = j;
                    min = recieve_buffers[j][indexes[j]];
                }
            }
            indexes[min_index]++;
            array[i] = min;

        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        // for (long long i = 0; i < numbers; i++)
        // {
        //     std::cout << array[i] << " ";
        // }
        // std::cout << std::endl;
        std::cout << commsize << " processes sorted " << numbers << " numbers in " << duration.count() / 1000000 << "." << std::setfill('0') << std::setw(6) << duration.count() % 1000000 << " seconds" << std::endl;

        // delete[] array;
    }
    else
    {
        int size = 0;
        MPI_Status status;
        MPI_Recv(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        int *array = new int[size];
        MPI_Recv(array, size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        sort(array, size);
        MPI_Send(array, size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
