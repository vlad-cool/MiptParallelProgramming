#!/bin/sh
./bin/main > /dev/null
# mpirun -np 1 ./bin/mpi > /dev/null
mpirun -np 2 ./bin/mpi > /dev/null
mpirun -np 4 ./bin/mpi > /dev/null
mpirun -np 8 ./bin/mpi > /dev/null
mpirun -np 12 ./bin/mpi > /dev/null
