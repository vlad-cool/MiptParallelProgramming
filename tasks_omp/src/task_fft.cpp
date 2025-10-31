#include <iostream>
#include <complex>
#include <cmath>
#include <numbers>
#include <iomanip>
#include <fstream>

using namespace std::complex_literals;

void ft(std::complex<double> *in, std::complex<double> *out, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        out[i] = 0;
        for (size_t j = 0; j < size; j++)
        {
            out[i] += in[j] * std::exp(-2i * std::numbers::pi * static_cast<double>(i * j) / static_cast<double>(size));
        }
    }
}

void fft(std::complex<double> *in, std::complex<double> *out, size_t size, size_t step = 1)
{
    if (size == 1)
    {
        out[0] = in[0];
        return;
    }

    std::complex<double> *tmp = new std::complex<double>[size];

    fft(in, tmp, size / 2, step * 2);
    fft(in + step, tmp + size / 2, size / 2, step * 2);

    for (size_t j = 0; j < size / 2; j++)
    {
        std::complex<double> omega_i = std::exp(-2i * std::numbers::pi * static_cast<double>(j) / static_cast<double>(size));
        std::complex<double> rhs = omega_i * tmp[size / 2 + j];

        out[j] = tmp[j] + rhs;
        out[size / 2 + j] = tmp[j] - rhs;
    }

    delete[] tmp;
}

void fft_omp(std::complex<double> *in, std::complex<double> *out, size_t size, size_t step = 1)
{
    if (size == 1)
    {
        out[0] = in[0];
        return;
    }

    std::complex<double> *tmp = new std::complex<double>[size];

    if (size > 16)
    {
        fft_omp(in, tmp, size / 2, step * 2);
        fft_omp(in + step, tmp + size / 2, size / 2, step * 2);
    }
    else
    {
        fft(in, tmp, size / 2, step * 2);
        fft(in + step, tmp + size / 2, size / 2, step * 2);
    }

#pragma omp parallel for
    for (size_t j = 0; j < size / 2; j++)
    {
        std::complex<double> omega_i = std::exp(-2i * std::numbers::pi * static_cast<double>(j) / static_cast<double>(size));
        std::complex<double> rhs = omega_i * tmp[size / 2 + j];

        out[j] = tmp[j] + rhs;
        out[size / 2 + j] = tmp[j] - rhs;
    }

    delete[] tmp;
}

int main(int argc, char *argv[])
{
    std::complex<double> *in_1 = new std::complex<double>[16];
    std::complex<double> *out_1 = new std::complex<double>[16];

    std::cout << std::fixed << std::setprecision(2);

    for (size_t i = 0; i < 16; i++)
    {
        in_1[i] = 0;
        if (i % 4 == 0)
        {
            in_1[i] = 1;
        }
        else if (i % 2 == 0)
        {
            in_1[i] = -1;
        }
        std::cout << in_1[i] << " ";
    }
    std::cout << std::endl;

    // ft(in, out, 16);
    // fft(in_1, out_1, 16, 1);
    // ft(in, out, 4);
    // fft(in_1, out_1, 4, 1);

    for (size_t i = 0; i < 16; i++)
    {
        std::cout << out_1[i] << " ";
    }
    std::cout << std::endl;

    delete[] in_1;
    delete[] out_1;

    if (argc < 3)
    {
        return -1;
    }

    size_t size = std::stol(argv[2]);

    std::ifstream f;
    f.open(argv[1]);

    std::complex<double> *in = new std::complex<double>[size];
    std::complex<double> *out_ref = new std::complex<double>[size];
    std::complex<double> *out = new std::complex<double>[size];

    for (size_t i = 0; i < size; i++)
    {
        std::string tmp;
        f >> tmp;
        in[i] = std::stod(tmp);
    }

    ft(in, out_ref, size);
    fft(in, out, size);
    fft_omp(in, out, size);

    double max_delta = 0;

    for (size_t i = 0; i < size; i++)
    {
        max_delta = std::max(max_delta, std::abs(out_ref[i] - out[i]));

        // std::cerr << out_ref[i] << " " << out[i] << "\n";
    }

    std::cout << "Max delta: " << max_delta << std::endl;
}
