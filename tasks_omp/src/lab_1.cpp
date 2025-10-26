#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>

int **allocate_matrix(size_t size)
{
    int *linear = new int[size * size];
    int **mat = new int *[size];

    for (size_t i = 0; i < size; i++)
    {
        mat[i] = linear + size * i;
    }

    return mat;
}

void free_matrix(int **matrix)
{
    delete[] matrix[0];
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

void matrix_multiply_stras(const size_t size, int **matr_1, int **matr_2, int **matr_res)
{
    if (size == 1)
    {
        **matr_res = **matr_1 * **matr_2;
        return;
    }

    int **A_1_1 = allocate_matrix(size / 2);
    int **A_1_2 = allocate_matrix(size / 2);
    int **A_2_1 = allocate_matrix(size / 2);
    int **A_2_2 = allocate_matrix(size / 2);

    int **B_1_1 = allocate_matrix(size / 2);
    int **B_1_2 = allocate_matrix(size / 2);
    int **B_2_1 = allocate_matrix(size / 2);
    int **B_2_2 = allocate_matrix(size / 2);

    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {

            A_1_1[i][j] = matr_1[i][j];
            A_1_2[i][j] = matr_1[i][j + size / 2];
            A_2_1[i][j] = matr_1[i + size / 2][j];
            A_2_2[i][j] = matr_1[i + size / 2][j + size / 2];

            B_1_1[i][j] = matr_2[i][j];
            B_1_2[i][j] = matr_2[i][j + size / 2];
            B_2_1[i][j] = matr_2[i + size / 2][j];
            B_2_2[i][j] = matr_2[i + size / 2][j + size / 2];
        }
    }

    int **D = allocate_matrix(size / 2);
    int **D_1 = allocate_matrix(size / 2);
    int **D_2 = allocate_matrix(size / 2);
    int **H_1 = allocate_matrix(size / 2);
    int **H_2 = allocate_matrix(size / 2);
    int **V_1 = allocate_matrix(size / 2);
    int **V_2 = allocate_matrix(size / 2);

    int **tmp_L = allocate_matrix(size / 2);
    int **tmp_R = allocate_matrix(size / 2);

    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A_1_1[i][j] + A_2_2[i][j];
            tmp_R[i][j] = B_1_1[i][j] + B_2_2[i][j];
        }
    }
    matrix_multiply_stras(size / 2, tmp_L, tmp_R, D);

    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A_1_2[i][j] - A_2_2[i][j];
            tmp_R[i][j] = B_2_1[i][j] + B_2_2[i][j];
        }
    }
    matrix_multiply_stras(size / 2, tmp_L, tmp_R, D_1);

    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A_2_1[i][j] - A_1_1[i][j];
            tmp_R[i][j] = B_1_1[i][j] + B_1_2[i][j];
        }
    }
    matrix_multiply_stras(size / 2, tmp_L, tmp_R, D_2);

    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A_1_1[i][j] + A_1_2[i][j];
        }
    }
    matrix_multiply_stras(size / 2, tmp_L, B_2_2, H_1);

    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_L[i][j] = A_2_1[i][j] + A_2_2[i][j];
        }
    }
    matrix_multiply_stras(size / 2, tmp_L, B_1_1, H_2);

    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_R[i][j] = B_2_1[i][j] - B_1_1[i][j];
        }
    }
    matrix_multiply_stras(size / 2, A_2_2, tmp_R, V_1);

    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            tmp_R[i][j] = B_1_2[i][j] - B_2_2[i][j];
        }
    }
    matrix_multiply_stras(size / 2, A_1_1, tmp_R, V_2);

    for (size_t i = 0; i < size / 2; i++)
    {
        for (size_t j = 0; j < size / 2; j++)
        {
            matr_res[i][j] = D[i][j] + D_1[i][j] + V_1[i][j] - H_1[i][j];
            matr_res[i][j + size / 2] = V_2[i][j] + H_1[i][j];
            matr_res[i + size / 2][j] = V_1[i][j] + H_2[i][j];
            matr_res[i + size / 2][j + size / 2] = D[i][j] + D_2[i][j] + V_2[i][j] - H_2[i][j];
        }
    }

    free_matrix(tmp_L);
    free_matrix(tmp_R);
    free_matrix(D_1);
    free_matrix(D_2);
    free_matrix(H_1);
    free_matrix(H_2);
    free_matrix(V_1);
    free_matrix(V_2);
}

int main()
{
    const size_t SIZE = 512;

    int **matr_1 = allocate_matrix(SIZE);
    int **matr_2 = allocate_matrix(SIZE);
    int **matr_res = allocate_matrix(SIZE);
    int **matr_ref = allocate_matrix(SIZE);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(-100, 100);

    for (size_t i = 0; i < SIZE; i++)
    {
        for (size_t j = 0; j < SIZE; j++)
        {
            matr_1[i][j] = dist(gen);
            matr_2[i][j] = dist(gen);
        }
    }

    // CACHE PREHEAT
    matrix_multiply_simple(SIZE, matr_1, matr_2, matr_ref);
    matrix_multiply_simple(SIZE, matr_1, matr_2, matr_ref);
    matrix_multiply_simple(SIZE, matr_1, matr_2, matr_ref);
    {
        auto start = std::chrono::high_resolution_clock::now();
        matrix_multiply_simple(SIZE, matr_1, matr_2, matr_ref);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        print_matrix(SIZE, matr_ref);
        std::cerr << "Matrices " << SIZE << "X" << SIZE << " multiplied simple way, time: " << duration.count() / 1000000 << "." << std::setfill('0') << std::setw(6) << duration.count() % 1000000 << " seconds" << std::endl;
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        matrix_multiply_transpose_opt(SIZE, matr_1, matr_2, matr_res);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        print_matrix(SIZE, matr_res);
        std::cerr << "Matrices "
                  << SIZE << "X" << SIZE
                  << " multiplied with transpose optimization"
                  << ", success: " << std::boolalpha << cmp_matrix(SIZE, matr_ref, matr_res)
                  << ", time: " << duration.count() / 1000000 << "." << std::setfill('0') << std::setw(6) << duration.count() % 1000000 << " seconds"
                  << std::endl;
    }

    for (size_t block_size = 1; block_size <= SIZE; block_size *= 2)
    {
        auto start = std::chrono::high_resolution_clock::now();
        matrix_multiply_block_opt(SIZE, block_size, matr_1, matr_2, matr_res);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        print_matrix(SIZE, matr_res);
        std::cerr << "Matrices "
                  << SIZE << "X" << SIZE
                  << " multiplied with block cache optimization, block size: " << block_size
                  << ", success: " << std::boolalpha << cmp_matrix(SIZE, matr_ref, matr_res)
                  << ", time: " << duration.count() / 1000000 << "." << std::setfill('0') << std::setw(6) << duration.count() % 1000000 << " seconds"
                  << std::endl;
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        matrix_multiply_simple(SIZE, matr_1, matr_2, matr_res);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        print_matrix(SIZE, matr_res);
        std::cerr << "Matrices "
                  << SIZE << "X" << SIZE
                  << " multiplied simple way"
                  << ", success: " << std::boolalpha << cmp_matrix(SIZE, matr_ref, matr_res)
                  << ", time: " << duration.count() / 1000000 << "." << std::setfill('0') << std::setw(6) << duration.count() % 1000000 << " seconds"
                  << std::endl;
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        matrix_multiply_stras(SIZE, matr_1, matr_2, matr_res);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        print_matrix(SIZE, matr_res);
        std::cerr << "Matrices "
                  << SIZE << "X" << SIZE
                  << " multiplied using Strassen algorithm"
                  << ", success: " << std::boolalpha << cmp_matrix(SIZE, matr_ref, matr_res)
                  << ", time: " << duration.count() / 1000000 << "." << std::setfill('0') << std::setw(6) << duration.count() % 1000000 << " seconds"
                  << std::endl;
    }

    free_matrix(matr_1);
    free_matrix(matr_2);
    free_matrix(matr_res);
    free_matrix(matr_ref);
}
