#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>
#include <functional>
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#include <x86intrin.h>

int **allocate_matrix(size_t size)
{
    int *linear = static_cast<int *>(std::aligned_alloc(32, sizeof(int) * size * size));
    int **mat = new int *[size];

    for (size_t i = 0; i < size; i++)
    {
        mat[i] = linear + size * i;
    }

    return mat;
}

void free_matrix(int **matrix)
{
    std::free(matrix[0]);
    delete[] matrix;
}

void print_matrix(size_t size, int **matrix)
{
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            std::cout << matrix[i][j] << ", ";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

bool cmp_matrix(size_t size, int **matrix_1, int **matrix_2)
{
    for (size_t i = 0; i < size * size; i++)
    {
        if (matrix_1[0][i] != matrix_2[0][i])
        {
            return false;
        }
    }
    return true;
}

void matrix_multiply_simple(size_t size, int **matr_1, int **matr_2, int **matr_res)
{
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            matr_res[i][j] = 0;
        }
    }

    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            for (size_t k = 0; k < size; k++)
            {
                matr_res[i][j] += matr_1[i][k] * matr_2[k][j];
            }
        }
    }
}

void matrix_multiply_transpose_opt(size_t size, int **matr_1, int **matr_2, int **matr_res)
{
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            matr_res[i][j] = 0;
        }
    }

    int **matr_tr = allocate_matrix(size);
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            matr_tr[i][j] = matr_2[j][i];
            // std::swap(matr_2[i][j], matr_2[j][i]);
        }
    }

    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            for (size_t k = 0; k < size; k++)
            {
                matr_res[i][j] += matr_1[i][k] * matr_tr[j][k];
            }
        }
    }

    free_matrix(matr_tr);
}

void matrix_multiply_block_opt(const size_t size, const size_t block_size, int **matr_1, int **matr_2, int **matr_res)
{
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            matr_res[i][j] = 0;
        }
    }

    for (size_t b_x = 0; b_x < size / block_size; b_x++)
    {
        for (size_t b_k = 0; b_k < size / block_size; b_k++)
        {
            for (size_t b_y = 0; b_y < size / block_size; b_y++)
            {
                for (size_t x = 0; x < block_size; x++)
                {
                    for (size_t y = 0; y < block_size; y++)
                    {
                        size_t i = b_x * block_size + x;
                        size_t j = b_y * block_size + y;

                        for (size_t z = 0; z < block_size; z++)
                        {
                            size_t k = b_k * block_size + z;
                            matr_res[i][j] += matr_1[i][k] * matr_2[k][j];
                        }
                    }
                }
            }
        }
    }
}

void matrix_multiply_stras(const size_t size, int **matr_1, int **matr_2, int **matr_res, size_t threshold = 8)
{
    if (size <= threshold)
    {
        matrix_multiply_transpose_opt(size, matr_1, matr_2, matr_res);
        return;
    }

    int **A = matr_1;
    int **B = matr_2;

    int **tmp = allocate_matrix(size / 2);
    int **tmp_L = allocate_matrix(size / 2);
    int **tmp_R = allocate_matrix(size / 2);

    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A[i][j] + A[i + size / 2][j + size / 2];
            tmp_R[i][j] = B[i][j] + B[i + size / 2][j + size / 2];
        }
    }
    matrix_multiply_stras(size / 2, tmp_L, tmp_R, tmp);
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            matr_res[i][j] = tmp[i][j];
            matr_res[i + size / 2][j + size / 2] = tmp[i][j];
        }
    }

    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A[i][j + size / 2] - A[i + size / 2][j + size / 2];
            tmp_R[i][j] = B[i + size / 2][j] + B[i + size / 2][j + size / 2];
        }
    }
    matrix_multiply_stras(size / 2, tmp_L, tmp_R, tmp);
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            matr_res[i][j] += tmp[i][j];
        }
    }

    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A[i + size / 2][j] - A[i][j];
            tmp_R[i][j] = B[i][j] + B[i][j + size / 2];
        }
    }
    matrix_multiply_stras(size / 2, tmp_L, tmp_R, tmp);
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            matr_res[i + size / 2][j + size / 2] += tmp[i][j];
        }
    }

    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A[i][j] + A[i][j + size / 2];
            tmp_R[i][j] = B[i + size / 2][j + size / 2];
        }
    }
    matrix_multiply_stras(size / 2, tmp_L, tmp_R, tmp);
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            matr_res[i][j] -= tmp[i][j];
            matr_res[i][j + size / 2] = tmp[i][j];
        }
    }

    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A[i + size / 2][j] + A[i + size / 2][j + size / 2];
            tmp_R[i][j] = B[i][j];
        }
    }
    matrix_multiply_stras(size / 2, tmp_L, tmp_R, tmp);
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            matr_res[i + size / 2][j] = tmp[i][j];
            matr_res[i + size / 2][j + size / 2] -= tmp[i][j];
        }
    }

    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A[i + size / 2][j + size / 2];
            tmp_R[i][j] = B[i + size / 2][j] - B[i][j];
        }
    }
    matrix_multiply_stras(size / 2, tmp_L, tmp_R, tmp);
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            matr_res[i][j] += tmp[i][j];
            matr_res[i + size / 2][j] += tmp[i][j];
        }
    }

    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A[i][j];
            tmp_R[i][j] = B[i][j + size / 2] - B[i + size / 2][j + size / 2];
        }
    }
    matrix_multiply_stras(size / 2, tmp_L, tmp_R, tmp);
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            matr_res[i][j + size / 2] += tmp[i][j];
            matr_res[i + size / 2][j + size / 2] += tmp[i][j];
        }
    }

    free_matrix(tmp);
    free_matrix(tmp_L);
    free_matrix(tmp_R);
}

void matrix_multiply_transpose_opt_omp(size_t size, int **matr_1, int **matr_2, int **matr_res)
{
#pragma omp for collapse(2)
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            matr_res[i][j] = 0;
        }
    }

    int **matr_tr = allocate_matrix(size);
#pragma omp for collapse(2)
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            matr_tr[i][j] = matr_2[j][i];
            // std::swap(matr_2[i][j], matr_2[j][i]);
        }
    }

#pragma omp for collapse(2)
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            for (size_t k = 0; k < size; k++)
            {
                matr_res[i][j] += matr_1[i][k] * matr_tr[j][k];
            }
        }
    }

    free_matrix(matr_tr);
}

void matrix_multiply_stras_omp(const size_t size, int **matr_1, int **matr_2, int **matr_res, size_t threshold = 8)
{
    if (size <= threshold)
    {
        matrix_multiply_transpose_opt(size, matr_1, matr_2, matr_res);
        return;
    }

    int **A = matr_1;
    int **B = matr_2;

    int **tmp = allocate_matrix(size / 2);
    int **tmp_L = allocate_matrix(size / 2);
    int **tmp_R = allocate_matrix(size / 2);

#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A[i][j] + A[i + size / 2][j + size / 2];
            tmp_R[i][j] = B[i][j] + B[i + size / 2][j + size / 2];
        }
    }
    matrix_multiply_stras_omp(size / 2, tmp_L, tmp_R, tmp);

#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            matr_res[i][j] = tmp[i][j];
            matr_res[i + size / 2][j + size / 2] = tmp[i][j];
        }
    }

#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A[i][j + size / 2] - A[i + size / 2][j + size / 2];
            tmp_R[i][j] = B[i + size / 2][j] + B[i + size / 2][j + size / 2];
        }
    }
    matrix_multiply_stras_omp(size / 2, tmp_L, tmp_R, tmp);
#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            matr_res[i][j] += tmp[i][j];
        }
    }

#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A[i + size / 2][j] - A[i][j];
            tmp_R[i][j] = B[i][j] + B[i][j + size / 2];
        }
    }
    matrix_multiply_stras_omp(size / 2, tmp_L, tmp_R, tmp);
#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            matr_res[i + size / 2][j + size / 2] += tmp[i][j];
        }
    }

#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A[i][j] + A[i][j + size / 2];
            tmp_R[i][j] = B[i + size / 2][j + size / 2];
        }
    }
    matrix_multiply_stras_omp(size / 2, tmp_L, tmp_R, tmp);
#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            matr_res[i][j] -= tmp[i][j];
            matr_res[i][j + size / 2] = tmp[i][j];
        }
    }

#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A[i + size / 2][j] + A[i + size / 2][j + size / 2];
            tmp_R[i][j] = B[i][j];
        }
    }
    matrix_multiply_stras_omp(size / 2, tmp_L, tmp_R, tmp);
#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            matr_res[i + size / 2][j] = tmp[i][j];
            matr_res[i + size / 2][j + size / 2] -= tmp[i][j];
        }
    }

#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A[i + size / 2][j + size / 2];
            tmp_R[i][j] = B[i + size / 2][j] - B[i][j];
        }
    }
    matrix_multiply_stras_omp(size / 2, tmp_L, tmp_R, tmp);
#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            matr_res[i][j] += tmp[i][j];
            matr_res[i + size / 2][j] += tmp[i][j];
        }
    }

#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A[i][j];
            tmp_R[i][j] = B[i][j + size / 2] - B[i + size / 2][j + size / 2];
        }
    }
    matrix_multiply_stras_omp(size / 2, tmp_L, tmp_R, tmp);
#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            matr_res[i][j + size / 2] += tmp[i][j];
            matr_res[i + size / 2][j + size / 2] += tmp[i][j];
        }
    }

    free_matrix(tmp);
    free_matrix(tmp_L);
    free_matrix(tmp_R);
}

// const __m256i shuffle_1 = _mm256_setr_epi32(7, 6, 5, 4, 3, 2, 1, 0);
// const __m256i shuffle_2 = _mm256_setr_epi32(4, 5, 6, 7, 1, 2, 3, 4);
// const __m256i shuffle_3 = _mm256_setr_epi32(6, 7, 4, 5, 2, 4, 0, 1);
const __m256i shuffle_1 = _mm256_setr_epi32(7, 0, 1, 2, 3, 4, 5, 6);
const __m256i shuffle_2 = _mm256_setr_epi32(6, 7, 0, 1, 2, 3, 4, 5);
const __m256i shuffle_3 = _mm256_setr_epi32(4, 5, 6, 7, 0, 1, 2, 3);

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

void matrix_multiply_transpose_opt_omp_simd(size_t size, int **matr_1, int **matr_2, int **matr_res)
{
    int **matr_tr = allocate_matrix(size);

#pragma omp for collapse(2)
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            matr_res[i][j] = 0;
        }
    }

#pragma omp for collapse(2)
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            matr_tr[i][j] = matr_2[j][i];
        }
    }

#pragma omp for collapse(2)
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            for (size_t k = 0; k < size; k += 8)
            {
                __m256i a = _mm256_load_si256((__m256i *)(matr_1[i] + k));
                __m256i b = _mm256_load_si256((__m256i *)(matr_tr[j] + k));
                __m256i tmp = _mm256_mullo_epi32(a, b);
                tmp = _mm256_add_epi32(tmp, _mm256_permutevar8x32_epi32(tmp, shuffle_1));
                tmp = _mm256_add_epi32(tmp, _mm256_permutevar8x32_epi32(tmp, shuffle_2));
                tmp = _mm256_add_epi32(tmp, _mm256_permutevar8x32_epi32(tmp, shuffle_3));

                alignas(32) __int32_t arr[8];
                _mm256_store_si256((__m256i *)arr, tmp);

                matr_res[i][j] += arr[0];
            }
        }
    }

    free_matrix(matr_tr);
}

void matrix_multiply_stras_omp_simd(const size_t size, int **matr_1, int **matr_2, int **matr_res, size_t threshold = 8)
{
    if (size <= threshold)
    {
        matrix_multiply_transpose_opt_omp_simd(size, matr_1, matr_2, matr_res);
        return;
    }

    int **A = matr_1;
    int **B = matr_2;

    int **tmp = allocate_matrix(size / 2);
    int **tmp_L = allocate_matrix(size / 2);
    int **tmp_R = allocate_matrix(size / 2);

#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A[i][j] + A[i + size / 2][j + size / 2];
            tmp_R[i][j] = B[i][j] + B[i + size / 2][j + size / 2];
        }
    }
    matrix_multiply_stras_omp_simd(size / 2, tmp_L, tmp_R, tmp);

#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            matr_res[i][j] = tmp[i][j];
            matr_res[i + size / 2][j + size / 2] = tmp[i][j];
        }
    }

#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A[i][j + size / 2] - A[i + size / 2][j + size / 2];
            tmp_R[i][j] = B[i + size / 2][j] + B[i + size / 2][j + size / 2];
        }
    }
    matrix_multiply_stras_omp_simd(size / 2, tmp_L, tmp_R, tmp);
#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            matr_res[i][j] += tmp[i][j];
        }
    }

#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A[i + size / 2][j] - A[i][j];
            tmp_R[i][j] = B[i][j] + B[i][j + size / 2];
        }
    }
    matrix_multiply_stras_omp_simd(size / 2, tmp_L, tmp_R, tmp);
#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            matr_res[i + size / 2][j + size / 2] += tmp[i][j];
        }
    }

#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A[i][j] + A[i][j + size / 2];
            tmp_R[i][j] = B[i + size / 2][j + size / 2];
        }
    }
    matrix_multiply_stras_omp_simd(size / 2, tmp_L, tmp_R, tmp);
#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            matr_res[i][j] -= tmp[i][j];
            matr_res[i][j + size / 2] = tmp[i][j];
        }
    }

#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A[i + size / 2][j] + A[i + size / 2][j + size / 2];
            tmp_R[i][j] = B[i][j];
        }
    }
    matrix_multiply_stras_omp_simd(size / 2, tmp_L, tmp_R, tmp);
#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            matr_res[i + size / 2][j] = tmp[i][j];
            matr_res[i + size / 2][j + size / 2] -= tmp[i][j];
        }
    }

#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A[i + size / 2][j + size / 2];
            tmp_R[i][j] = B[i + size / 2][j] - B[i][j];
        }
    }
    matrix_multiply_stras_omp_simd(size / 2, tmp_L, tmp_R, tmp);
#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            matr_res[i][j] += tmp[i][j];
            matr_res[i + size / 2][j] += tmp[i][j];
        }
    }

#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A[i][j];
            tmp_R[i][j] = B[i][j + size / 2] - B[i + size / 2][j + size / 2];
        }
    }
    matrix_multiply_stras_omp_simd(size / 2, tmp_L, tmp_R, tmp);
#pragma omp for collapse(2)
    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            matr_res[i][j + size / 2] += tmp[i][j];
            matr_res[i + size / 2][j + size / 2] += tmp[i][j];
        }
    }

    free_matrix(tmp);
    free_matrix(tmp_L);
    free_matrix(tmp_R);
}

void matrix_multiply_transpose_opt_omp_outer(size_t size, int **matr_1, int **matr_2, int **matr_res)
{
#pragma omp parallel
    {
        matrix_multiply_transpose_opt_omp_simd(size, matr_1, matr_2, matr_res);
    }
}

void matrix_multiply_stras_omp_outer(size_t size, int **matr_1, int **matr_2, int **matr_res)
{
#pragma omp parallel
    {
        matrix_multiply_stras_omp_simd(size, matr_1, matr_2, matr_res);
    }
}

void matrix_multiply_transpose_opt_omp_simd_outer(size_t size, int **matr_1, int **matr_2, int **matr_res)
{
#pragma omp parallel
    {
        matrix_multiply_transpose_opt_omp_simd(size, matr_1, matr_2, matr_res);
    }
}

void matrix_multiply_stras_omp_simd_outer(size_t size, int **matr_1, int **matr_2, int **matr_res)
{
#pragma omp parallel
    {
        matrix_multiply_stras_omp_simd(size, matr_1, matr_2, matr_res);
    }
}

void test_method(std::string name, size_t size, int **matr_1, int **matr_2, int **matr_res, int **matr_ref, std::function<void(size_t, int **, int **, int **)> test_func)
{
    auto start = std::chrono::high_resolution_clock::now();
    test_func(size, matr_1, matr_2, matr_res);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    if (!cmp_matrix(size, matr_ref, matr_res))
    {
        std::cerr << "Wrong answer, method: " << name << ", size: " << size << std::endl;
    }

    std::cout << name << ", " << size << ", " << duration.count() / 1000000 << "." << std::setfill('0') << std::setw(6) << duration.count() % 1000000 << std::endl;
}

int main()
{
    // size_t size = 64;
    // for (size_t size = 64; size <= 256; size *= 2)
    for (size_t size = 64; size <= 2048; size *= 2)
    {

        int **matr_1 = allocate_matrix(size);
        int **matr_2 = allocate_matrix(size);
        int **matr_res = allocate_matrix(size);
        int **matr_ref = allocate_matrix(size);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(-100, 100);

        for (size_t i = 0; i < size; i++)
        {
            for (size_t j = 0; j < size; j++)
            {
                matr_1[i][j] = dist(gen);
                matr_2[i][j] = dist(gen);
            }
        }

        matrix_multiply_simple(size, matr_1, matr_2, matr_ref);

        test_method("simple", size, matr_1, matr_2, matr_res, matr_ref, matrix_multiply_simple);
        test_method("transpose_opt", size, matr_1, matr_2, matr_res, matr_ref, matrix_multiply_transpose_opt);
        for (size_t block_size = 1; block_size <= size; block_size *= 2)
        {
            std::function<void(size_t, int **, int **, int **)> test_func = [block_size](size_t size, int **matr_1, int **matr_2, int **matr_res)
            { matrix_multiply_block_opt(size, block_size, matr_1, matr_2, matr_res); };
            std::stringstream ss;
            ss << "block_opt_" << block_size;
            test_method(ss.str(), size, matr_1, matr_2, matr_res, matr_ref, test_func);
        }
        for (size_t threshold = 1; threshold <= size; threshold *= 2)
        {
            std::function<void(size_t, int **, int **, int **)> test_func = [threshold](size_t size, int **matr_1, int **matr_2, int **matr_res)
            { matrix_multiply_stras(size, matr_1, matr_2, matr_res, threshold); };
            std::stringstream ss;
            ss << "strassen_" << threshold;
            test_method(ss.str(), size, matr_1, matr_2, matr_res, matr_ref, test_func);
        }
        for (size_t threshold = 8; threshold <= size; threshold *= 2)
        {
            std::function<void(size_t, int **, int **, int **)> test_func = [threshold](size_t size, int **matr_1, int **matr_2, int **matr_res)
            { matrix_multiply_stras_omp(size, matr_1, matr_2, matr_res, threshold); };
            std::stringstream ss;
            ss << "strassen_omp_" << threshold;
            test_method(ss.str(), size, matr_1, matr_2, matr_res, matr_ref, test_func);
        }
        for (size_t threshold = 8; threshold <= size; threshold *= 2)
        {
            std::function<void(size_t, int **, int **, int **)> test_func = [threshold](size_t size, int **matr_1, int **matr_2, int **matr_res)
            { matrix_multiply_stras_omp_simd(size, matr_1, matr_2, matr_res, threshold); };
            std::stringstream ss;
            ss << "strassen_omp_simd_" << threshold;
            test_method(ss.str(), size, matr_1, matr_2, matr_res, matr_ref, test_func);
        }

        test_method("transpose_opt_omp_simd", size, matr_1, matr_2, matr_res, matr_ref, matrix_multiply_transpose_opt_omp_simd);

        free_matrix(matr_1);
        free_matrix(matr_2);
        free_matrix(matr_res);
        free_matrix(matr_ref);
    }
}
