#!/bin/bash

cd $(dirname $0)/..

echo "base:"
bin/task_1_base data/task_1_base
echo "mpi (x4):"
mpirun -np 4 bin/task_1_mpi data/task_1_mpi_4
echo "mpi (x5):"
mpirun -np 5 bin/task_1_mpi data/task_1_mpi_5
echo "mpi (x6):"
mpirun -np 6 bin/task_1_mpi data/task_1_mpi_6
echo "mpi (x7):"
mpirun -np 7 bin/task_1_mpi data/task_1_mpi_7
echo "mpi (x8):"
mpirun -np 8 bin/task_1_mpi data/task_1_mpi_8
echo "mpi (x12):"
mpirun -np 16 bin/task_1_mpi data/task_1_mpi_12
echo "mpi (x16):"
mpirun -np 16 bin/task_1_mpi data/task_1_mpi_16

diff data/task_1_base data/task_1_mpi_4
diff data/task_1_base data/task_1_mpi_5
diff data/task_1_base data/task_1_mpi_6
diff data/task_1_base data/task_1_mpi_7
diff data/task_1_base data/task_1_mpi_8
diff data/task_1_base data/task_1_mpi_12
diff data/task_1_base data/task_1_mpi_16
