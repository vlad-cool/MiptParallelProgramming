#include <chrono>
#include <thread>
#include <sstream>
#include <functional>
#include <algorithm>
#include <iomanip>

#include "gol_single.h"
#include "gol_shared.h"
#include "gol_mpi.h"
#include "gol_hybrid.h"

int main(int argc, char *argv[])
{
    /*
    -w WIDTH
    -h HIGH
    -s STEPS
    -m {SINGLE, SHARED, MPI, HYBRID}
    -p {NUMBER_OF_THREADS} (for shared and hybrid (per mpi process))
    -o (optimize mpi send) // TODO
    -a (print all steps)
    -H (print hash)
    -f (print field)
    -t (print calculation time)
    -i (print launch info)
    -d DELAY (in milliseconds, make sense with -a -f options to make good animation)
    -r SEED (seed for random, default random_device())
    -P PERCENT (percentage for random fill)
    */
    std::random_device rd;

    uint32_t width = 128;
    uint32_t height = 16;
    uint32_t steps = 1;
    uint32_t number_of_threads = 1;
    bool optimize_mpi = true;
    bool print_every_step = false;
    bool print_hash = false;
    bool print_field = false;
    bool print_calc_time = false;
    bool print_launch_info = false;
    uint32_t delay = 0;
    uint32_t seed = rd();
    uint32_t percentage = 33;

    int commsize, my_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

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
            std::cout << "-o (optimize mpi send) // TODO\n";
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

    GameOfLife *gol = nullptr;

    for (int i = 1; i < argc; i++)
    {
        std::string arg = std::string(argv[i]);
        if (arg == "-m")
        {
            if (i + 1 == argc)
            {
                std::cout << "No mode argument, use -m MODE" << std::endl;
                return 1;
            }
            std::string mode = std::string(argv[i + 1]);
            std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);
            if (mode == "single")
            {
                if (my_rank != 0)
                {
                    MPI_Finalize();
                    return 0;
                }
                if (print_launch_info)
                {
                    std::cout << "Running single threaded\n";
                }
                gol = new GameOfLife(width, height);
            }
            if (mode == "shared")
            {
                if (my_rank != 0)
                {
                    MPI_Finalize();
                    return 0;
                }
                if (print_launch_info)
                {
                    std::cout << "Running " << number_of_threads << " threads\n";
                }
                gol = new GameOfLifeShared(width, height, number_of_threads);
            }
            if (mode == "mpi")
            {
                if (print_launch_info)
                {
                    std::cout << "Running " << commsize << " mpi processes\n";
                }
                gol = new GameOfLifeMpi(width, height, commsize, my_rank);
            }
            if (mode == "hybrid")
            {

                if (print_launch_info)
                {
                    std::cout << "Running " << commsize << " mpi processes, each" << number_of_threads << " threads\n";
                }
                gol = new GameOfLifeHybrid(width, height, commsize, my_rank, number_of_threads);
            }
            break;
        }
    }
    if (gol == nullptr)
    {
        if (my_rank != 0)
        {
            MPI_Finalize();
            return 0;
        }
        if (print_launch_info)
        {
            std::cout << "Running single threaded\n";
        }
        gol = new GameOfLife(width, height);
    }

    if (print_launch_info)
    {
        std::cout << std::boolalpha;
        std::cout << "Field: " << width << "x" << height << ", steps: " << steps << "\n";
        std::cout << "Send optimization " << optimize_mpi << "\n";
        std::cout << "Random seed: " << seed << "\n";
        std::cout << "Start field density" << percentage << "%";
        std::cout << std::endl;
    }

    std::mt19937 gen(seed);
    std::uniform_int_distribution<> distrib(0, 99);
    int random_num = distrib(gen);
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            gol->set_cell(x, y, distrib(gen) < percentage);
        }
    }

    std::chrono::microseconds duration = std::chrono::microseconds(0);

    for (int i = 0; i < steps; i++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        gol->step();
        auto end = std::chrono::high_resolution_clock::now();
        duration += std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        if (my_rank == 0)
        {
            std::cerr << "step #" << i + 1 << " of " << steps << std::endl;
        }

        if (print_every_step || i + 1 == steps)
        {
            if (print_field)
            {
                gol->print(std::cout);
            }
            if (print_hash)
            {
                std::ostringstream oss;
                std::hash<std::string> hasher;
                gol->print(oss);
                if (my_rank == 0)
                {
                    std::string result = oss.str();
                    std::cout << "hash: 0x" << std::hex << std::setw(16) << hasher(result) << std::dec << "\n";
                }
            }
        }

        if (my_rank == 0)
        {
            std::cout << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }

    if (print_calc_time)
    {
        std::cout << "Calculation time: " << duration.count() / 1000000 << "." << std::setfill('0') << std::setw(6) << duration.count() % 1000000 << " seconds" << std::endl;
    }

    MPI_Finalize();
}
