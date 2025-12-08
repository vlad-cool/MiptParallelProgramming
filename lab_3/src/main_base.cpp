#include <iostream>
#include <cmath>
#include <chrono>

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

    auto start = std::chrono::high_resolution_clock::now();
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

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << duration.count() / 1000000 << "." << std::setfill('0') << std::setw(6) << duration.count() % 1000000 << std::endl;

    std::cerr << -10 << ", " << sqrt(2) << "\n";
    for (ssize_t i = 0; i < N; i++)
    {
        std::cerr << -10 + 20.0 * (i + 1.0) / (N + 1) << ", " << y[i] << "\n";
    }
    std::cerr << 10 << ", " << sqrt(2) << "\n";
    std::cerr << std::endl;
}
