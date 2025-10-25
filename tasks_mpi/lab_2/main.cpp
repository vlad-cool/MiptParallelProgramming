#include <iostream>
#include <thread>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <atomic>
#include <list>
#include <mutex>
#include <iomanip>
#include <cassert>
#include <chrono>

double f(double x)
{
    // return x * x;
    // return std::cos(x);
    return std::cos(1 / x);
}

// double start = 0;
// double stop = 1;

double start = 1 / 100;
double stop = 8;

uint32_t number_of_threads = 8;
size_t points_per_interval = 8192;

size_t next_worker;
std::atomic<size_t> tasks_counter;

std::mutex i_mtx;
std::mutex res_mtx;

std::mutex dbg_mtx;

double res = 0;
double epsilon = 1e-5;

struct IntegrationInterval
{
    double start;
    double end;
    size_t n;

    IntegrationInterval()
    {
        start = 0.0;
        end = 0.0;
        n = points_per_interval;
    }

    IntegrationInterval(double start, double end, size_t n)
    {
        this->start = start;
        this->end = end;
        this->n = n;
    }
};

struct IntegrationResult
{
    double integral;
    double delta;

    IntegrationResult()
    {
        integral = 0.0;
        delta = 0.0;
    }

    IntegrationResult(double integral, double delta)
    {
        this->integral = integral;
        this->delta = delta;
    }
};

double integrate_trapezoid(double from, double to, size_t intervals)
{
    double step = (to - from) / intervals;
    double res = 0;
    for (size_t i = 1; i < intervals; i++)
    {
        res += f(from + i * step - step / 2) * step;
    }
    return res;
}

IntegrationResult integrate(IntegrationInterval &interval)
{
    double integral_1 = integrate_trapezoid(interval.start, interval.end, interval.n);
    double integral_2 = integrate_trapezoid(interval.start, interval.end, interval.n * 2);

    assert(integral_1 == integral_1);
    assert(integral_2 == integral_2);

    return IntegrationResult(integral_2, std::abs(integral_1 - integral_2) * (stop - start) / (interval.end - interval.start + epsilon));
}

std::vector<std::list<IntegrationInterval>> intervals;

void thread_handler(size_t thread_id)
{
    size_t a_w = tasks_counter.load();
    while (a_w != 0)
    {
        // dbg_mtx.lock();
        // std::cout << "      " << thread_id << std::endl;
        // dbg_mtx.unlock();
        i_mtx.lock();
        if (intervals[thread_id].size() == 0)
        {
            i_mtx.unlock();
            // dbg_mtx.lock();
            // std::cout << "aaa " << thread_id << " " << a_w << std::endl;
            // dbg_mtx.unlock();
            tasks_counter.wait(a_w);
            // dbg_mtx.lock();
            // std::cout << "bbb " << thread_id << " " << a_w << std::endl;
            // dbg_mtx.unlock();
        }
        else
        {
            IntegrationInterval interval = intervals[thread_id].front();
            intervals[thread_id].pop_front();
            i_mtx.unlock();

            IntegrationResult result = integrate(interval);

            if (result.delta < epsilon)
            {
                i_mtx.lock();
                res += result.integral;
                tasks_counter.fetch_sub(1);
                tasks_counter.notify_all();
                i_mtx.unlock();
            }
            else
            {
                i_mtx.lock();

                if (next_worker == thread_id)
                {
                    next_worker += 1;
                    next_worker %= number_of_threads;
                }
                intervals[next_worker].push_back(
                    IntegrationInterval(
                        interval.start,
                        (interval.end + interval.start) / 2,
                        interval.n * 2));
                next_worker += 1;
                next_worker %= number_of_threads;

                if (next_worker == thread_id)
                {
                    next_worker += 1;
                    next_worker %= number_of_threads;
                }
                intervals[next_worker].push_back(
                    IntegrationInterval(
                        (interval.end + interval.start) / 2,
                        interval.end,
                        interval.n * 2));
                next_worker += 1;
                next_worker %= number_of_threads;

                tasks_counter.fetch_add(1);
                tasks_counter.notify_all();

                i_mtx.unlock();
            }
        }

        a_w = tasks_counter.load();
    }
}

int main(int argc, char *argv[])
{
    bool suppress_res = false;

    for (int i = 1; i < argc; i++)
    {
        std::string arg = std::string(argv[i]);
        if (arg == "--help")
        {
            std::cout << "-t THREADS\n";
            std::cout << "-p PRECISION\n";
            std::cout << "-s - suppres integral value\n";
            std::cout << std::endl;
            return 0;
        }
        if (arg == "-t")
        {
            if (i + 1 == argc)
            {
                std::cout << "No threads argument, use -t THREADS" << std::endl;
                return 1;
            }
            else
            {
                number_of_threads = std::stoi(argv[i + 1]);
                i++;
            }
        }
        else if (arg == "-p")
        {
            if (i + 1 == argc)
            {
                std::cout << "No precision argument, use -p PRECISION" << std::endl;
                return 1;
            }
            else
            {
                int precision = std::stoi(argv[i + 1]) + 1;
                epsilon = std::pow(10, -precision);
                i++;
            }
        }
        else if (arg == "-s")
        {
            suppress_res = true;
        }
    }

    auto t_start = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threads;
    for (size_t i = 0; i < number_of_threads; i++)
    {
        intervals.push_back(std::list<IntegrationInterval>(0));
        intervals[i].push_back(
            IntegrationInterval(
                start + i * (stop - start) / number_of_threads,
                start + (i + 1) * (stop - start) / number_of_threads,
                points_per_interval));
    }
    tasks_counter.store(number_of_threads);

    threads.reserve(number_of_threads);
    for (size_t i = 0; i < number_of_threads; i++)
    {
        threads.push_back(
            std::thread([i]()
                        { thread_handler(i); }));
    }

    size_t a_w = tasks_counter.load();
    while (a_w != 0)
    {
        tasks_counter.wait(a_w);
        a_w = tasks_counter.load();
    }

    for (size_t i = 0; i < number_of_threads; i++)
    {
        threads[i].join();
    }
    auto t_end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start).count();
    std::cout << number_of_threads << " " << duration / 1000000 << "." << std::setfill('0') << std::setw(6) << duration % 1000000 << " seconds" << std::endl;

    if (!suppress_res)
    {
        std::cout << std::setprecision(16) << res << std::endl;
        std::cout << std::setprecision(16) << std::fixed << epsilon * 10 << std::endl;
    }
}
