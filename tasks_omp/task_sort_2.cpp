#include <iostream>
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#include <x86intrin.h>
#include <bits/stdc++.h>
#include <random>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <chrono>

void print_m256i(__m256i n)
{
    alignas(32) __int32_t arr[8];
    _mm256_store_si256((__m256i *)arr, n);
    for (int i = 0; i < 8; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

const int perm_1_a[8] = {7, 0, 1, 2, 3, 4, 5, 6};
const __m256i perm_1 = _mm256_loadu_si256((__m256i *)perm_1_a);

const int mask_1_a[8] = {-1, 0, 0, 0, 0, 0, 0, 0};
const __m256i mask_1 = _mm256_loadu_si256((__m256i *)mask_1_a);

void sort_8(__int32_t *arr)
{
    __m256i a = _mm256_loadu_si256((__m256i *)arr);
    __m256i b = _mm256_loadu_si256((__m256i *)arr);
    __m256i permutation = _mm256_setzero_si256();

    for (int i = 0; i < 7; i++)
    {
        b = _mm256_permutevar8x32_epi32(b, perm_1);
        __m256i res = _mm256_cmpgt_epi32(a, b);
        permutation = _mm256_sub_epi32(permutation, res);
        const __m256i c = _mm256_setr_epi32(-1, 0, 1, 2, 3, 4, 5, 6);
        __m256i d = _mm256_set1_epi32(i);
        __m256i mask = _mm256_cmpgt_epi32(d, c);
        res = _mm256_cmpeq_epi32(a, b);
        res = _mm256_and_si256(res, mask);
        permutation = _mm256_sub_epi32(permutation, res);
    }

    alignas(32) __int32_t arr_1[8];
    alignas(32) __int32_t perm[8];
    _mm256_storeu_si256((__m256i *)arr_1, permutation);
    for (int i = 0; i < 8; i++)
    {
        perm[arr_1[i]] = i;
    }
    permutation = _mm256_loadu_si256((__m256i *)perm);

    __m256i res = _mm256_permutevar8x32_epi32(a, permutation);
    _mm256_storeu_si256((__m256i *)arr, res);
}

bool merge_sort(__int32_t *arr, __int32_t *buf, size_t size)
{
    if (size == 8)
    {
        sort_8(arr);
        return true;
    }
    else
    {
        size_t half_size = size / 2;

        __int32_t *arr_1 = arr;
        __int32_t *arr_2 = arr + half_size;
        __int32_t *buf_1 = buf;
        __int32_t *buf_2 = buf + half_size;

        bool is_arr = merge_sort(arr_1, buf_1, half_size);
        merge_sort(arr_2, buf_2, half_size);

        __int32_t *src_1, *src_2, *dst;

        if (is_arr)
        {
            src_1 = arr_1;
            src_2 = arr_2;
            dst = buf;
        }
        else
        {
            src_1 = buf_1;
            src_2 = buf_2;
            dst = arr;
        }

        size_t i_1 = 0, i_2 = 0, i = 0;
        while (i_1 < half_size && i_2 < half_size)
        {
            if (src_1[i_1] < src_2[i_2])
            {
                dst[i] = src_1[i_1];
                i_1++;
                i++;
            }
            else
            {
                dst[i] = src_2[i_2];
                i_2++;
                i++;
            }
        }
        while (i_1 < half_size)
        {

            dst[i] = src_1[i_1];
            i_1++;
            i++;
        }
        while (i_2 < half_size)
        {
            dst[i] = src_2[i_2];
            i_2++;
            i++;
        }

        return !is_arr;
    }
}

int main()
{
    for (int i = 0; i < 1; i++)
    {
        size_t size = 1024 * 1024;
        // size_t size = 1024 * 1024 * 128;

        __int32_t *array = new int32_t[size];
        __int32_t *array_ref = new int32_t[size];

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 1024 * 256);

        for (size_t i = 0; i < size; i++)
        {
            array[i] = dist(gen);
            array_ref[i] = array[i];
        }

        auto start = std::chrono::high_resolution_clock::now();

        __int32_t *buf = new int32_t[size];
        bool res = merge_sort(array, buf, size);
        if (!res) {
            delete[] array;
            array = buf;
        }
        else {
            delete[] buf;
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cerr << "Sorted using merge + simd " << size << " numbers in " << duration.count() / 1000000 << "." << std::setfill('0') << std::setw(6) << duration.count() % 1000000 << " seconds" << std::endl;

        start = std::chrono::high_resolution_clock::now();

        std::sort(array_ref, array_ref + size);


        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cerr << "Sorted using std sort " << size << " numbers in " << duration.count() / 1000000 << "." << std::setfill('0') << std::setw(6) << duration.count() % 1000000 << " seconds" << std::endl;

        for (size_t i = 0; i < size; i++)
        {
            // std::cout << array[i] << " " << array_ref[i] << std::endl;
            assert(array[i] == array_ref[i]);
        }
        for (size_t i = 0; i < size; i++)
        {
            std::cout << array[i] << " " << array_ref[i] << std::endl;
            // assert(array[i] == array_ref[i]);
        }
    }

    return 0;
}
