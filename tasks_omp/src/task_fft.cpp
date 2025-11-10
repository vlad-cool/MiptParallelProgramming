#include <iostream>
#include <complex>
#include <cmath>
#include <numbers>
#include <iomanip>
#include <fstream>
#include <omp.h>
#include <x86intrin.h>
#include <functional>
#include <chrono>

#pragma GCC target("avx2")
#pragma GCC optimize("O3")

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

void fft(std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, size_t size, size_t step = 1)
{
    if (size == 1)
    {
        out[0] = in[0];
        return;
    }

    fft(in, tmp, out, size / 2, step * 2);
    fft(in + step, tmp + size / 2, out + size / 2, size / 2, step * 2);

    for (size_t j = 0; j < size / 2; j++)
    {
        std::complex<double> omega_i = std::exp(-2i * std::numbers::pi * static_cast<double>(j) / static_cast<double>(size));
        std::complex<double> rhs = omega_i * tmp[size / 2 + j];

        out[j] = tmp[j] + rhs;
        out[size / 2 + j] = tmp[j] - rhs;
    }
}

void fft_simd(std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, size_t size, size_t step = 1)
{
    if (size == 1)
    {
        out[0] = in[0];
        return;
    }

    fft_simd(in, tmp, out, size / 2, step * 2);
    fft_simd(in + step, tmp + size / 2, out + size / 2, size / 2, step * 2);

    if (size == 8)
    {
        alignas(32) double omega_re[4];
        alignas(32) double omega_im[4];
        alignas(32) double tmp_re[4];
        alignas(32) double tmp_im[4];

        for (size_t i = 0; i < 4; i++)
        {
            omega_re[i] = std::cos(-2 * std::numbers::pi * static_cast<double>(i) / static_cast<double>(size));
            omega_im[i] = std::sin(-2 * std::numbers::pi * static_cast<double>(i) / static_cast<double>(size));
            tmp_re[i] = tmp[size / 2 + i].real();
            tmp_im[i] = tmp[size / 2 + i].imag();
        }

        __m256d omega_re_v = _mm256_load_pd(omega_re);
        __m256d omega_im_v = _mm256_load_pd(omega_im);
        __m256d tmp_re_v = _mm256_load_pd(tmp_re);
        __m256d tmp_im_v = _mm256_load_pd(tmp_im);

        __m256d rhs_re = _mm256_sub_pd(_mm256_mul_pd(omega_re_v, tmp_re_v), _mm256_mul_pd(omega_im_v, tmp_im_v));
        __m256d rhs_im = _mm256_add_pd(_mm256_mul_pd(omega_re_v, tmp_im_v), _mm256_mul_pd(omega_im_v, tmp_re_v));

        for (size_t i = 0; i < 4; i++)
        {
            tmp_re[i] = tmp[i].real();
            tmp_im[i] = tmp[i].imag();
        }

        tmp_re_v = _mm256_load_pd(tmp_re);
        tmp_im_v = _mm256_load_pd(tmp_im);

        __m256d res_1_re_v = _mm256_add_pd(tmp_re_v, rhs_re);
        __m256d res_1_im_v = _mm256_add_pd(tmp_im_v, rhs_im);

        __m256d res_2_re_v = _mm256_sub_pd(tmp_re_v, rhs_re);
        __m256d res_2_im_v = _mm256_sub_pd(tmp_im_v, rhs_im);

        alignas(32) double res_re_1[4], res_re_2[4];
        alignas(32) double res_im_1[4], res_im_2[4];

        _mm256_store_pd(res_re_1, res_1_re_v);
        _mm256_store_pd(res_im_1, res_1_im_v);
        _mm256_store_pd(res_re_2, res_2_re_v);
        _mm256_store_pd(res_im_2, res_2_im_v);

        // __m256d tmp_1 = _mm256_mul_pd()

        for (size_t i = 0; i < 4; i++)
        {
            out[i] = res_re_1[i] + 1i * res_im_1[i];
            out[i + size / 2] = res_re_2[i] + 1i * res_im_2[i];
        }

        return;
    }

    for (size_t j = 0; j < size / 2; j++)
    {
        std::complex<double> omega_i = std::exp(-2i * std::numbers::pi * static_cast<double>(j) / static_cast<double>(size));
        std::complex<double> rhs = omega_i * tmp[size / 2 + j];

        out[j] = tmp[j] + rhs;
        out[size / 2 + j] = tmp[j] - rhs;
    }
}

void fft_omp_for_inner(std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, size_t size, size_t step = 1)
{
    if (size == 1)
    {
        out[0] = in[0];
        return;
    }

    fft_omp_for_inner(in, tmp, out, size / 2, step * 2);
    fft_omp_for_inner(in + step, tmp + size / 2, out + size / 2, size / 2, step * 2);

#pragma omp for
    for (size_t j = 0; j < size / 2; j++)
    {
        std::complex<double> omega_i = std::exp(-2i * std::numbers::pi * static_cast<double>(j) / static_cast<double>(size));
        std::complex<double> rhs = omega_i * tmp[size / 2 + j];

        out[j] = tmp[j] + rhs;
        out[size / 2 + j] = tmp[j] - rhs;
    }
}

void fft_omp_for(std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, size_t size)
{
#pragma omp parallel
    {
// #pragma omp single
        {
            fft_omp_for_inner(in, out, tmp, size);
        }
    }
}

void fft_omp_for_simd_inner(std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, size_t size, size_t step = 1)
{
    if (size == 1)
    {
        out[0] = in[0];
        return;
    }

    if (size == 8)
    {
        fft_simd(in, out, tmp, size, step);
    }

    fft_omp_for_simd_inner(in, tmp, out, size / 2, step * 2);
    fft_omp_for_simd_inner(in + step, tmp + size / 2, out + size / 2, size / 2, step * 2);

#pragma omp for
    for (size_t j = 0; j < size / 2; j++)
    {
        std::complex<double> omega_i = std::exp(-2i * std::numbers::pi * static_cast<double>(j) / static_cast<double>(size));
        std::complex<double> rhs = omega_i * tmp[size / 2 + j];

        out[j] = tmp[j] + rhs;
        out[size / 2 + j] = tmp[j] - rhs;
    }
}


void fft_omp_for_simd(std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, size_t size)
{
#pragma omp parallel
    {
// #pragma omp single
        {
            fft_omp_for_simd_inner(in, out, tmp, size);
        }
    }
}


void fft_omp_task_inner(size_t depth, std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, size_t size, size_t step = 1)
{
    if (size == 1)
    {
        out[0] = in[0];
        return;
    }

    if (depth > 0)
    {
#pragma omp task
        fft_omp_task_inner(depth - 1, in, tmp, out, size / 2, step * 2);
#pragma omp task
        fft_omp_task_inner(depth - 1, in + step, tmp + size / 2, out + size / 2, size / 2, step * 2);
#pragma omp taskwait
    }
    else
    {
        fft(in, tmp, out, size / 2, step * 2);
        fft(in + step, tmp + size / 2, out + size / 2, size / 2, step * 2);
    }

    for (size_t j = 0; j < size / 2; j++)
    {
        std::complex<double> omega_i = std::exp(-2i * std::numbers::pi * static_cast<double>(j) / static_cast<double>(size));
        std::complex<double> rhs = omega_i * tmp[size / 2 + j];

        out[j] = tmp[j] + rhs;
        out[size / 2 + j] = tmp[j] - rhs;
    }
}

void fft_omp_task(std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, size_t size, size_t depth = 4)
{
#pragma omp parallel
    {
#pragma omp single
        {
            fft_omp_task_inner(depth, in, out, tmp, size, 1);
        }
    }
}

void fft_omp_task_inner_simd(size_t depth, std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, size_t size, size_t step = 1)
{
    if (size == 1)
    {
        out[0] = in[0];
        return;
    }

    if (size == 8)
    {
        fft_simd(in, out, tmp, size, step);
    }

    if (depth > 0)
    {
#pragma omp task
        fft_omp_task_inner_simd(depth - 1, in, tmp, out, size / 2, step * 2);
#pragma omp task
        fft_omp_task_inner_simd(depth - 1, in + step, tmp + size / 2, out + size / 2, size / 2, step * 2);
#pragma omp taskwait
    }
    else
    {
        fft_simd(in, tmp, out, size / 2, step * 2);
        fft_simd(in + step, tmp + size / 2, out + size / 2, size / 2, step * 2);
    }

    for (size_t j = 0; j < size / 2; j++)
    {
        std::complex<double> omega_i = std::exp(-2i * std::numbers::pi * static_cast<double>(j) / static_cast<double>(size));
        std::complex<double> rhs = omega_i * tmp[size / 2 + j];

        out[j] = tmp[j] + rhs;
        out[size / 2 + j] = tmp[j] - rhs;
    }
}

void fft_omp_task_simd(std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, size_t size, size_t depth = 4)
{
#pragma omp parallel
    {
#pragma omp single
        {
            fft_omp_task_inner_simd(depth, in, out, tmp, size, 1);
        }
    }
}

void test_method(std::string name, std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, std::complex<double> *ref, size_t size, std::function<void(std::complex<double> *, std::complex<double> *, std::complex<double> *, size_t)> test_func)
{
    auto start = std::chrono::high_resolution_clock::now();
    test_func(in, out, tmp, size);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    double max_delta = 0;

    for (size_t i = 0; i < size; i++)
    {
        max_delta = std::max(max_delta, std::abs(ref[i] - out[i]));
    }

    std::cout << name << ", " << size << ", " << duration.count() / 1000000 << "." << std::setfill('0') << std::setw(6) << duration.count() % 1000000 << ", " << max_delta << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Not enough arguments" << std::endl;
        return -1;
    }

    std::cout << std::fixed << std::setprecision(2);

    size_t size = std::stoull(argv[2]);

    std::ifstream f;
    f.open(argv[1]);

    std::complex<double> *in = new std::complex<double>[size];
    std::complex<double> *tmp = new std::complex<double>[size];
    std::complex<double> *out_ref = new std::complex<double>[size];
    std::complex<double> *out = new std::complex<double>[size];

    for (size_t i = 0; i < size; i++)
    {
        std::string tmp;
        f >> tmp;
        in[i] = std::stod(tmp);
    }

    for (size_t size = 64; size <= 1024 * 1024 * 4; size *= 2)
    {
        // ft(in, out_ref, size);
        fft(in, out_ref, tmp, size);

        std::function<void(std::complex<double> *, std::complex<double> *, std::complex<double> *, size_t)> test_func;
        
        test_func = [](std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, size_t size)
        { fft(in, out, tmp, size); };
        test_method("fft", in, out, tmp, out_ref, size, test_func);

        test_func = [](std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, size_t size)
        { fft_simd(in, out, tmp, size); };
        test_method("fft_simd", in, out, tmp, out_ref, size, test_func);

        // test_func = [](std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, size_t size)
        // { fft_omp_for(in, out, tmp, size); };
        // test_method("fft_omp_for", in, out, tmp, out_ref, size, test_func);

        // test_func = [](std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, size_t size)
        // { fft_omp_for_simd(in, out, tmp, size); };
        // test_method("fft_omp_for_simd", in, out, tmp, out_ref, size, test_func);

        test_func = [](std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, size_t size)
        { fft_omp_task(in, out, tmp, size); };
        test_method("fft_task", in, out, tmp, out_ref, size, test_func);

        test_func = [](std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, size_t size)
        { fft_omp_task_simd(in, out, tmp, size); };
        test_method("fft_task_simd", in, out, tmp, out_ref, size, test_func);

        test_func = [](std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, size_t size)
        { fft_omp_task_simd(in, out, tmp, size, 2); };
        test_method("fft_task_simd_2", in, out, tmp, out_ref, size, test_func);

        test_func = [](std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, size_t size)
        { fft_omp_task_simd(in, out, tmp, size, 3); };
        test_method("fft_task_simd_3", in, out, tmp, out_ref, size, test_func);

        test_func = [](std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, size_t size)
        { fft_omp_task_simd(in, out, tmp, size, 4); };
        test_method("fft_task_simd_4", in, out, tmp, out_ref, size, test_func);

        test_func = [](std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, size_t size)
        { fft_omp_task_simd(in, out, tmp, size, 5); };
        test_method("fft_task_simd_5", in, out, tmp, out_ref, size, test_func);

        test_func = [](std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, size_t size)
        { fft_omp_task_simd(in, out, tmp, size, 6); };
        test_method("fft_task_simd_6", in, out, tmp, out_ref, size, test_func);

        test_func = [](std::complex<double> *in, std::complex<double> *out, std::complex<double> *tmp, size_t size)
        { fft_omp_task_simd(in, out, tmp, size, 7); };
        test_method("fft_task_simd_7", in, out, tmp, out_ref, size, test_func);
    }

    delete[] in;
    delete[] tmp;
    delete[] out_ref;
    delete[] out;
}
