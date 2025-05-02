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
std::mutex w_c_mtx;
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
    // assert(step == step);
    // dbg_mtx.lock();
    // std::cout << "step " << step << std::endl;
    // dbg_mtx.unlock();
    double res = 0;
    for (size_t i = 1; i < intervals; i++)
    {
        // assert(f(from + i * step) == f(from + i * step));
        // assert(f(from + (i - 1) * step) == f(from + (i - 1) * step));
        // res += (f(from + i * step) + f(from + i * step - step)) / 2 * step;
        res += f(from + i * step - step / 2) * step;
    }
    // assert(res == res);
    return res;
}

IntegrationResult integrate(IntegrationInterval &interval)
{
    // dbg_mtx.lock();
    // std::cout << "     " << interval.start << " " << interval.end << " " << interval.end - interval.start << " " << interval.n << std::endl;
    // dbg_mtx.unlock();

    double integral_1 = integrate_trapezoid(interval.start, interval.end, interval.n);
    double integral_2 = integrate_trapezoid(interval.start, interval.end, interval.n * 2);

    assert(integral_1 == integral_1);
    assert(integral_2 == integral_2);

    // std::cout << integral_1 << " " << integral_2 << " " << std::abs(integral_1 - integral_2) << std::endl;
    return IntegrationResult(integral_2, std::abs(integral_1 - integral_2) * (stop - start) / (interval.end - interval.start + epsilon));
}

std::vector<std::list<IntegrationInterval>> intervals;

void thread_handler(size_t thread_id)
{
    size_t a_w = tasks_counter.load();
    while (a_w != 0)
    {
        i_mtx.lock();
        // dbg_mtx.lock();
        // std::cout << "     " << thread_id << " " << intervals[thread_id].size() << " " << a_w << std::endl;
        // dbg_mtx.unlock();
        if (intervals[thread_id].size() == 0)
        {
            i_mtx.unlock();

            // dbg_mtx.lock();
            // std::cout << thread_id << " aa " << a_w << std::endl;
            // dbg_mtx.unlock();
            tasks_counter.wait(a_w);
            // dbg_mtx.lock();
            // std::cout << thread_id << " bb " << tasks_counter.load() << std::endl;
            // dbg_mtx.unlock();
        }
        else
        {
            IntegrationInterval interval = intervals[thread_id].front();
            i_mtx.unlock();

            IntegrationResult result = integrate(interval);
            
            // dbg_mtx.lock();
            // std::cout << "delta: " << result.delta << std::endl;
            // dbg_mtx.unlock();

            if (result.delta < epsilon)
            {
                res_mtx.lock();
                res += result.integral;
                res_mtx.unlock();
                i_mtx.lock();
                intervals[thread_id].pop_front();

                // dbg_mtx.lock();
                // std::cout << thread_id << " sub 1" << std::endl;
                // dbg_mtx.unlock();
                tasks_counter.fetch_sub(1);
                tasks_counter.notify_all();
                i_mtx.unlock();
            }
            else
            {
                w_c_mtx.lock();
                i_mtx.lock();

                intervals[next_worker].push_back(
                    IntegrationInterval(
                        interval.start,
                        (interval.end + interval.start) / 2,
                        interval.n * 2));
                next_worker += 1;
                next_worker %= number_of_threads;

                intervals[next_worker].push_back(
                    IntegrationInterval(
                        (interval.end + interval.start) / 2,
                        interval.end,
                        interval.n * 2));
                next_worker += 1;
                next_worker %= number_of_threads;

                intervals[thread_id].pop_front();

                // dbg_mtx.lock();
                // std::cout << thread_id << " add 1" << std::endl;
                // dbg_mtx.unlock();
                tasks_counter.fetch_add(1);
                tasks_counter.notify_all();

                i_mtx.unlock();
                w_c_mtx.unlock();
            }
        }

        a_w = tasks_counter.load();
    }

    // dbg_mtx.lock();
    // std::cout << thread_id << " exited" << std::endl;
    // dbg_mtx.unlock();
}

int main(int argc, char *argv[])
{
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

    // dbg_mtx.lock();
    // std::cout << "fgdfg " << std::setprecision(16) << res << std::endl;
    // dbg_mtx.unlock();

    for (size_t i = 0; i < number_of_threads; i++)
    {
        threads[i].join();
    }

    std::cout << std::setprecision(16) << res << std::endl;
    std::cout << std::setprecision(16) << std::fixed << epsilon << std::endl;
}
