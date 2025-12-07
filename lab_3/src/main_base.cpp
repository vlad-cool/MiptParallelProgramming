#include <iostream>

double a;

double f(double x, double y)
{
    return a * (y - y * y * y);
}

double get_element(double *a, ssize_t i, ssize_t size)
{
    if (i < 0)
    {
        return 0;
    }
    if (i >= size)
    {
        return 0;
    }
    return a[i];
}

void set_element(double *a, double value, ssize_t i, ssize_t size)
{
    if (i < 0)
    {
        return;
    }
    if (i >= size)
    {
        return;
    }
    a[i] = value;
}

double solve_tridiag(double *diag_0, double *diag_1, double *diag_2, double *b, double *res, ssize_t size)
{
    double max = 0;

    for (ssize_t i = 1; i < size; i++)
    {
        double c = diag_0[i - 1] / diag_1[i - 1];

        diag_1[i] -= diag_2[i - 1] * c;

        b[i] -= b[i - 1] * c;
    }

    for (ssize_t i = size - 2; i + 1 > 0; i--)
    {
        double c = diag_2[i] / diag_1[i + 1];

        b[i] -= b[i + 1] * c;
    }

    for (ssize_t i = 0; i < size; i++)
    {
        res[i] = b[i] / diag_1[i];
    }

    return max;
}

double solve_reduction(double *diag_0, double *diag_1, double *diag_2, double *rhs, double *res, ssize_t size, ssize_t height)
{
    if (height == 0 || size < 2)
    {
        return solve_tridiag(diag_0, diag_1, diag_2, rhs, res, size);
    }

    double *diag_0_0, *diag_0_1, *diag_0_2, *diag_1_0, *diag_1_1, *diag_1_2, *rhs_0, *rhs_1, *res_0, *res_1;

    ssize_t size_0, size_1;

    size_1 = size / 2;
    size_0 = size - size_1;

    diag_0_0 = new double[size_0 - 1];
    diag_0_1 = new double[size_0];
    diag_0_2 = new double[size_0 - 1];
    rhs_0 = new double[size_0];
    res_0 = new double[size_0];

    diag_1_0 = new double[size_1 - 1];
    diag_1_1 = new double[size_1];
    diag_1_2 = new double[size_1 - 1];
    rhs_1 = new double[size_1];
    res_1 = new double[size_1];

    for (ssize_t j = 0; j < size_0; j++)
    {
        double a, b, c, d, e, f, g, h, i, x, y, z, u, v, w, q;

        a = get_element(diag_0, j * 2 - 2, size - 1);
        b = get_element(diag_1, j * 2 - 1, size);
        c = get_element(diag_2, j * 2 - 1, size - 1);

        d = get_element(diag_0, j * 2 - 1, size - 1);
        e = get_element(diag_1, j * 2 + 0, size);
        f = get_element(diag_2, j * 2 + 0, size - 1);

        g = get_element(diag_0, j * 2 + 0, size - 1);
        h = get_element(diag_1, j * 2 + 1, size);
        i = get_element(diag_2, j * 2 + 1, size - 1);

        u = get_element(rhs, j * 2 - 1, size);
        v = get_element(rhs, j * 2 + 0, size);
        w = get_element(rhs, j * 2 + 1, size);

        y = e;
        q = v;

        if (b != 0)
        {
            x = -a * d / b;
            y -= c * d / b;
            q -= u * d / b;
        }
        else
        {
            x = 0;
        }

        if (h != 0)
        {
            z = -i * f / h;
            y -= g * f / h;
            q -= w * f / h;
        }
        else
        {
            z = 0;
        }

        set_element(diag_0_0, x, j - 1, size_0 - 1);
        set_element(diag_0_1, y, j + 0, size_0);
        set_element(diag_0_2, z, j + 0, size_0 - 1);
        set_element(rhs_0, q, j + 0, size_0);
    }

    for (ssize_t j = 0; j < size_1; j++)
    {
        double a, b, c, d, e, f, g, h, i, x, y, z, u, v, w, q;

        a = get_element(diag_0, j * 2 - 1, size - 1);
        b = get_element(diag_1, j * 2 + 0, size);
        c = get_element(diag_2, j * 2 + 0, size - 1);

        d = get_element(diag_0, j * 2 + 0, size - 1);
        e = get_element(diag_1, j * 2 + 1, size);
        f = get_element(diag_2, j * 2 + 1, size - 1);

        g = get_element(diag_0, j * 2 + 1, size - 1);
        h = get_element(diag_1, j * 2 + 2, size);
        i = get_element(diag_2, j * 2 + 2, size - 1);

        u = get_element(rhs, j * 2 + 0, size);
        v = get_element(rhs, j * 2 + 1, size);
        w = get_element(rhs, j * 2 + 2, size);

        y = e;
        q = v;

        if (b != 0)
        {
            x = -a * d / b;
            y -= c * d / b;
            q -= u * d / b;
        }
        else
        {
            x = 0;
        }

        if (h != 0)
        {
            z = -i * f / h;
            y -= g * f / h;
            q -= w * f / h;
        }
        else
        {
            z = 0;
        }

        set_element(diag_1_0, x, j - 1, size_1 - 1);
        set_element(diag_1_1, y, j + 0, size_1);
        set_element(diag_1_2, z, j + 0, size_1 - 1);
        set_element(rhs_1, q, j + 0, size_1);
    }

    double max_1, max_2;

    max_1 = solve_reduction(diag_0_0, diag_0_1, diag_0_2, rhs_0, res_0, size_0, height - 1);
    max_2 = solve_reduction(diag_1_0, diag_1_1, diag_1_2, rhs_1, res_1, size_1, height - 1);

    for (ssize_t i = 0; i < size; i += 2)
    {
        res[i] = res_0[i / 2];
    }
    for (ssize_t i = 1; i < size; i += 2)
    {
        res[i] = res_1[i / 2];
    }

    delete[] diag_0_0;
    delete[] diag_0_1;
    delete[] diag_0_2;
    delete[] rhs_0;
    delete[] res_0;

    delete[] diag_1_0;
    delete[] diag_1_1;
    delete[] diag_1_2;
    delete[] rhs_1;
    delete[] res_1;

    return std::max(max_1, max_2);
}

int main(int argc, char *argv[])
{
    ssize_t N = 231;
    double **a = new double *[N];

    double *diag_0 = new double[N - 1];
    double *diag_1 = new double[N];
    double *diag_2 = new double[N - 1];

    a[0] = new double[N * N];

    for (ssize_t i = 1; i < N; i++)
    {
        a[i] = a[i - 1] + N;
    }

    double *b_0 = new double[N];
    double *b = new double[N];
    double *res = new double[N];

    for (ssize_t i = 0; i < N; i++)
    {
        for (ssize_t j = 0; j < N; j++)
        {
            if (i == j)
            {
                a[i][j] = 2 * i + 10;
                // a[i][j] = 2;
            }
            else if (i + 1 == j)
            {
                a[i][j] = 1 / (static_cast<double>(i) + 2);
                // a[i][j] = 1;
            }
            else if (j + 1 == i)
            {
                a[i][j] = 4 / (static_cast<double>(i) + 2);
                // a[i][j] = 1;
            }
            else
            {
                a[i][j] = 0;
            }
        }
        b[i] = i + 2;
    }

    for (ssize_t i = 0; i < N - 1; i++)
    {
        diag_0[i] = a[i + 1][i];
        diag_2[i] = a[i][i + 1];
    }

    for (ssize_t i = 0; i < N; i++)
    {
        diag_1[i] = a[i][i];
        b_0[i] = b[i];
    }

    // for (ssize_t i = 0; i < N; i++)
    // {
    //     for (ssize_t j = 0; j < N; j++)
    //     {
    //         std::cout << a[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // 0.666667 0.466667 0.504762 0.555556 0.545455 0.538462 0.533333 0.529412 0.526316 0.52381
    // 0.666667 0.466667 0.571429 0.555556 0.545455 0.538462 0.533333 0.529412 0.526316 0.52381

    // solve_tridiag(a, b, res, N);
    // solve_tridiag(diag_0, diag_1, diag_2, b, res, N);
    solve_reduction(diag_0, diag_1, diag_2, b, res, N, 1);

    std::cout << "=====" << std::endl;
    for (ssize_t i = 0; i < N; i++)
    {
        std::cout << res[i] << " ";
    }
    std::cout << "\n=====" << std::endl;

    for (ssize_t i = 0; i < N; i++)
    {
        double tmp = 0;
        for (ssize_t j = 0; j < N; j++)
        {
            tmp += a[i][j] * res[j];
        }
        tmp -= b_0[i];

        if (tmp < 1e-10)
        {
            tmp = 0;
        }
        std::cout << tmp << " ";
    }
    std::cout << std::endl;

    // double test_0[20] = {1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 1, 2, 1, 2};
    // double save_0[20] = {1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 1, 2, 1, 2};
    // double test_1[20] = {9, 8, 7, 6, 9, 8, 7, 6, 9, 8, 7, 6, 9, 8, 7, 6, 9, 8, 7, 6};
    // double save_1[20] = {9, 8, 7, 6, 9, 8, 7, 6, 9, 8, 7, 6, 9, 8, 7, 6, 9, 8, 7, 6};
    // double test_2[20] = {2, 2, 1, 2, 0, 1, 0, 1, 2, 1, 3, 2, 1, 2, 1, 2, 1, 1, 2};
    // double save_2[20] = {2, 2, 1, 2, 0, 1, 0, 1, 2, 1, 3, 2, 1, 2, 1, 2, 1, 1, 2};
    // double test_b[20] = {1, 2, 3, 4, 4, 3, 2, 1, 1, 2, 3, 4, 5, 5, 4, 3, 2, 1, 2, 3};
    // double save_b[20] = {1, 2, 3, 4, 4, 3, 2, 1, 1, 2, 3, 4, 5, 5, 4, 3, 2, 1, 2, 3};
    // double res[20];
    // double test_2[19] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    // double save_2[19] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    // double test_0[19] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // double save_0[19] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // double test_1[20] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    // double save_1[20] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    // double test_2[19] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // double save_2[19] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // double test_2[19] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    // double save_2[19] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    // double test_b[20] = {9, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    // double save_b[20] = {9, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    // double res[20];

    // solve_tridiag(test_0, test_1, test_2, test_b, res, 20);

    // std::cout << res[0] * save_1[0] + res[1] * save_2[0] - save_b[0] << std::endl;
    // for (ssize_t i = 1; i < 19; i++)
    // {
    //     std::cout << res[i - 1] * save_0[i] + res[i] * save_1[i] + res[i + 1] * save_2[i + 1] - save_b[i] << std::endl;
    //     ;
    // }
    // std::cout << res[18] * save_0[18] + res[19] * save_1[19] - save_b[19] << std::endl;

    // if (argc < 3)
    // {
    //     std::cout << "Not enough arguments" << std::endl;
    //     return 1;
    // }

    // a = std::stod(argv[1]);
    // ssize_t n = std::stoull(argv[2]);

    // ssize_t N = n * 16;

    // double x_s = -10, x_f = 10;

    // double h = (x_f - x_s) / N;

    // ////////////////////

    // double *diag_0, *diag_1, *diag_2, *b, *y;

    // diag_0 = new double[N - 1];
    // diag_1 = new double[N];
    // diag_2 = new double[N - 1];

    // b = new double[N];
    // y = new double[N];

    // for (ssize_t i = 0; i < N - 1; i++)
    // {
    //     diag_0[i] = 1;
    //     diag_1[i] = -2;
    //     diag_2[i] = 1;
    // }

    // diag_1[0] = 1;
    // diag_1[N - 1] = 1;

    // for (ssize_t i = 1; i < N - 1; i++)
    // {
    //     y[i] = 1;
    // }

    // delete[] diag_0;
    // delete[] diag_1;
    // delete[] diag_2;
    // delete[] b;
}