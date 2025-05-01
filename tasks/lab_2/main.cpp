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

uint32_t number_of_threads = 4;
size_t points_per_interval = 512;

size_t worker_counter;
std::atomic<size_t> active_workers;

std::mutex i_mtx;
std::mutex w_c_mtx;
std::mutex res_mtx;

std::mutex dbg_mtx;

double res = 0;
double epsilon = 1e-10;

struct IntegrationInterval
{
    double start;
    double end;

    IntegrationInterval()
    {
        start = 0.0;
        end = 0.0;
    }

    IntegrationInterval(double start, double end)
    {
        this->start = start;
        this->end = end;
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

IntegrationResult integrate(IntegrationInterval &interval)
{
    size_t n = points_per_interval;
    n += 1;
    n /= 2;
    n *= 2;
    double step = (interval.end - interval.start) / n;

    double integral_1 = 0;
    double integral_2 = 0;

    for (size_t i = 0; i + 1 < n; i++)
    {
        integral_1 += f(interval.start + step * i + step / 2) * step;
    }

    for (size_t i = 0; i < n * 2; i++)
    {
        integral_2 += f(interval.start + step * i / 2 + step / 4) * step / 2;
    }

    return IntegrationResult(integral_2, std::abs(integral_1 - integral_2) * 2 * (interval.end - interval.start) / (stop - start));
}

std::vector<std::list<IntegrationInterval>> intervals;

void thread_handler(size_t thread_id)
{
    while (active_workers.load() != 0)
    {
        dbg_mtx.lock();
        std::cout << "     " << thread_id << " " << active_workers.load() << std::endl;
        dbg_mtx.unlock();
        i_mtx.lock();
        if (intervals[thread_id].size() == 0)
        {
            i_mtx.unlock();
            size_t a_w = active_workers.load();
            if (a_w == 0)
            {
                break;
            }

            dbg_mtx.lock();
            std::cout << thread_id << " aa " << a_w << std::endl;
            dbg_mtx.unlock();
            active_workers.wait(a_w);
            dbg_mtx.lock();
            std::cout << thread_id << " bb " << active_workers.load() << std::endl;
            dbg_mtx.unlock();
            continue;
        }
        else
        {
            IntegrationInterval interval = intervals[thread_id].front();
            intervals[thread_id].pop_front();
            if (intervals[thread_id].size() == 0)
            {
                // std::cout << "ASDASD " << thread_id << " " << active_workers.load() << std::endl;
                active_workers.fetch_add(-1);
                // std::cout << "NMNMNM " << thread_id << " " << active_workers.load() << std::endl;
                active_workers.notify_all();
            }

            i_mtx.unlock();
            IntegrationResult result = integrate(interval);
            dbg_mtx.lock();
            std::cout << thread_id << " integrated" << std::endl;
            dbg_mtx.unlock();
            if (result.delta < epsilon)
            {
                res_mtx.lock();
                res += result.integral;
                res_mtx.unlock();
            }
            else
            {
                dbg_mtx.lock();
                std::cout << result.delta << std::endl;
                dbg_mtx.unlock();
                w_c_mtx.lock();
                i_mtx.lock();

                intervals[worker_counter].push_back(
                    IntegrationInterval(
                        interval.start,
                        interval.start + (interval.end - interval.start) / 2));
                if (intervals[worker_counter].size() == 1)
                {
                    active_workers.fetch_add(1);
                    active_workers.notify_all();
                }
                worker_counter += 1;
                worker_counter %= number_of_threads;

                intervals[worker_counter].push_back(
                    IntegrationInterval(
                        interval.start + (interval.end - interval.start) / 2,
                        interval.end));
                if (intervals[worker_counter].size() == 1)
                {
                    active_workers.fetch_add(1);
                    active_workers.notify_all();
                }
                worker_counter += 1;
                worker_counter %= number_of_threads;

                // if (intervals[thread_id].size() == 0)
                // {
                //     // std::cout << "ASDASD " << thread_id << " " << active_workers.load() << std::endl;
                //     active_workers.fetch_add(-1);
                //     // std::cout << "NMNMNM " << thread_id << " " << active_workers.load() << std::endl;
                //     active_workers.notify_all();
                // }

                i_mtx.unlock();
                w_c_mtx.unlock();
            }
        }
    }
    // i_mtx.unlock();
    // if ()
    // if (unfinished_tasks[0].load() == 0)
    // {
    //     bool ended = true;
    //     for (size_t i = 0; i < number_of_threads; i++)
    //     {
    //         if (unfinished_tasks[i].load() != 0)
    //         {
    //             ended = false;
    //             break;
    //         }
    //     }
    //     if (ended)
    //     {
    //         return;
    //     }
    // }
    // else
    // {

    // }
}

// threads.reserve(threads_number);
//     thread_running = new std::atomic<bool>(threads_number);
//     size_t from = 0;
//     for (size_t i = 0; i < threads_number; i++)
//     {
//         size_t step = height / threads_number + (i < (height % threads_number));
//         threads.push_back(
//             std::thread([from, step, i, this]()
//                         { this->partial_step(from, from + step, i); }));
//         thread_running[i].store(false);
//         from += step;
//     }

int main(int argc, char *argv[])
{
    // double h = 1e-3;
    // double epsilon = 1e-5;
    //
    /*
    for (int i = 1; i < argc; i++)
    {
        std::string arg = std::string(argv[i]);
        if (arg == "--help")
        {
            std::cout << "-w WIDTH\n";
            std::cout << "-h HIGH\n";
            std::cout << "-s STEPS\n";
            std::cout << "-m {SINGLE, SHARED, MPI, HYBRID}\n";
            std::cout << "-p {NUMBER_OF_THREADS} (for shared and hybrid (per mpi process))\n";
            std::cout << "-o (optimize mpi send)\n";
            std::cout << "-a (print all steps)\n";
            std::cout << "-H (print hash)\n";
            std::cout << "-f (print field)\n";
            std::cout << "-t (print calculation time)\n";
            std::cout << "-i (print launch info)\n";
            std::cout << "-d DELAY (in milliseconds, make sense with -a -f options to make good animation)\n";
            std::cout << "-r SEED (seed for random, default random_device())\n";
            std::cout << "-P PERCENT (percentage for random fill)";
            std::cout << std::endl;
            return 0;
        }
        if (arg == "-w")
        {
            if (i + 1 == argc)
            {
                std::cout << "No width argument, use -w WIDTH" << std::endl;
                return 1;
            }
            else
            {
                width = std::stoi(argv[i + 1]);
                i++;
            }
        }
        else if (arg == "-h")
        {
            if (i + 1 == argc)
            {
                std::cout << "No height argument, use -h HEIGHT" << std::endl;
                return 1;
            }
            else
            {
                height = std::stoi(argv[i + 1]);
                i++;
            }
        }
        else if (arg == "-s")
        {
            if (i + 1 == argc)
            {
                std::cout << "No steps argument, use -s STEPS" << std::endl;
                return 1;
            }
            else
            {
                steps = std::stoi(argv[i + 1]);
                i++;
            }
        }
        else if (arg == "-p")
        {
            if (i + 1 == argc)
            {
                std::cout << "No threads argument, use -p THREADS" << std::endl;
                return 1;
            }
            else
            {
                number_of_threads = std::stoi(argv[i + 1]);
                i++;
            }
        }
        else if (arg == "-d")
        {
            if (i + 1 == argc)
            {
                std::cout << "No delay argument, use -d DELAY" << std::endl;
                return 1;
            }
            else
            {
                delay = std::stoi(argv[i + 1]);
                i++;
            }
        }
        else if (arg == "-r")
        {
            if (i + 1 == argc)
            {
                std::cout << "No seed argument, use -r SEED" << std::endl;
                return 1;
            }
            else
            {
                seed = std::stoi(argv[i + 1]);
                i++;
            }
        }
        else if (arg == "-P")
        {
            if (i + 1 == argc)
            {
                std::cout << "No percentage argument, use -P PERCENRAGE" << std::endl;
                return 1;
            }
            else
            {
                percentage = std::stoi(argv[i + 1]);
                i++;
            }
        }
        else if (arg == "-o")
        {
            optimize_mpi = true;
        }
        else if (arg == "-a")
        {
            print_every_step = true;
        }
        else if (arg == "-H")
        {
            print_hash = true;
        }
        else if (arg == "-f")
        {
            print_field = true;
        }
        else if (arg == "-t")
        {
            print_calc_time = true;
        }
        else if (arg == "-i")
        {
            print_launch_info = true;
        }
    }
    */

    std::vector<std::thread> threads;
    // intervals = std::vector<std::list<IntegrationInterval>>(number_of_threads);
    // intervals.reserve(number_of_threads);
    // unfinished_tasks.reserve(number_of_threads);
    for (size_t i = 0; i < number_of_threads; i++)
    {
        intervals.push_back(std::list<IntegrationInterval>(0));
        intervals[i].push_back(
            IntegrationInterval{
                start + i * (stop - start) / number_of_threads,
                start + (i + 1) * (stop - start) / number_of_threads,
                // h,
                // i,
                // false,
                // 0.0,
                // 0.0
            });
        // unfinished_tasks.push_back(1);
    }
    active_workers.store(number_of_threads);
    std::cout << active_workers.load() << std::endl;

    threads.reserve(number_of_threads);
    for (size_t i = 0; i < number_of_threads; i++)
    {
        threads.push_back(
            std::thread([i]()
                        { thread_handler(i); }));
    }

    size_t a_w = active_workers.load();
    while (a_w != 0)
    {
        active_workers.wait(a_w);
        a_w = active_workers.load();
    }

    std::cout << res << std::endl;

    for (size_t i = 0; i < number_of_threads; i++)
    {
        threads[i].join();
    }

    std::cout << std::setprecision(16) << res << std::endl;
    std::cout << std::setprecision(16) << epsilon << std::endl;
}
