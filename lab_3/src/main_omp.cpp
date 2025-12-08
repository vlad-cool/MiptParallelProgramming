#include <iostream>
#include <cmath>

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

void solve_tridiag(double *diag_0, double *diag_1, double *diag_2, double *b, double *res, ssize_t size)
{
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
}

void solve_reduction(double *diag_0, double *diag_1, double *diag_2, double *rhs, double *res, ssize_t size, ssize_t height)
{
    if (height == 0 || size <= 2)
    {
        solve_tridiag(diag_0, diag_1, diag_2, rhs, res, size);
        return;
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

#pragma omp parallel
    {
#pragma omp for nowait
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

#pragma omp for
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

#pragma omp task
        solve_reduction(diag_0_0, diag_0_1, diag_0_2, rhs_0, res_0, size_0, height - 1);
#pragma omp task
        solve_reduction(diag_1_0, diag_1_1, diag_1_2, rhs_1, res_1, size_1, height - 1);
#pragma omp taskwait
    }

#pragma omp parallel
    {
#pragma omp for nowait
        for (ssize_t i = 0; i < size; i += 2)
        {
            res[i] = res_0[i / 2];
        }
#pragma omp for
        for (ssize_t i = 1; i < size; i += 2)
        {
            res[i] = res_1[i / 2];
        }
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
}

double a;

double f(double y)
{
    return a * (y * y * y - y);
}

double df(double y)
{
    return a * (3 * y * y - 1);
}

double F(double y_0, double y_1, double y_2, double h)
{
    return (f(y_0) + 10 * f(y_1) + f(y_2)) / 12 - (y_0 - 2 * y_1 + y_2) / (h * h);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " a h" << std::endl;
        return 1;
    }

    double h;
    double eps = 1e-7;

    a = std::stod(argv[1]);
    h = std::stod(argv[2]);

    ssize_t N = static_cast<ssize_t>(std::ceil(20 / h));

    double *diag_0 = new double[N - 1];
    double *diag_1 = new double[N];
    double *diag_2 = new double[N - 1];
    double *rhs = new double[N];
    double *y = new double[N];

    double *res = new double[N];

    for (ssize_t i = 0; i < N; i++)
    {
        y[i] = sqrt(2);
    }

    double max = eps * 2;

    while (max > eps)
    {
        for (ssize_t i = 0; i < N - 1; i++)
        {
            diag_0[i] = df(y[i]) / 12 - 1 / (h * h);
        }
        for (ssize_t i = 0; i < N; i++)
        {
            diag_1[i] = 10 * df(y[i]) / 12 + 2 / (h * h);
        }
        for (ssize_t i = 0; i < N - 1; i++)
        {
            diag_2[i] = df(y[i + 1]) / 12 - 1 / (h * h);
        }
        for (ssize_t i = 1; i < N - 1; i++)
        {
            rhs[i] = -F(y[i - 1], y[i], y[i + 1], h);
        }
        rhs[0] = -F(sqrt(2), y[0], y[1], h);
        rhs[N - 1] = -F(y[N - 2], y[N - 1], sqrt(2), h);

        // solve_reduction(diag_0, diag_1, diag_2, rhs, res, N, 4);
        solve_tridiag(diag_0, diag_1, diag_2, rhs, res, N);

        for (ssize_t i = 0; i < N; i++)
        {
            y[i] += res[i];
        }

        max = std::abs(res[0]);
        for (ssize_t i = 1; i < N; i++)
        {
            max = std::max(max, std::abs(res[i]));
        }
    }

    std::cerr << -10 << ", " << sqrt(2) << "\n";
    for (ssize_t i = 0; i < N; i++)
    {
        std::cerr << -10 + 20.0 * (i + 1.0) / (N + 1) << ", " << y[i] << "\n";
    }
    std::cerr << 10 << ", " << sqrt(2) << "\n";
    std::cerr << std::endl;
}
