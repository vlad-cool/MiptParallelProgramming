#!/bin/bash

cd $(dirname $0)/..

echo "base:"
bin/main_base data/main_base
echo "omp:"
bin/main_omp data/main_omp
echo "mpi (x8):"
mpirun -np 8 bin/main_mpi data/main_mpi
echo "mpi (x16):"
mpirun -np 16 bin/main_mpi data/main_mpi

diff data/main_base data/main_omp
diff data/main_base data/main_mpi
/main_base data/main_omp
diff data/main_base data/main_mpi
