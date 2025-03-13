#!/bin/sh

SCRIPT_DIR=$(realpath $(dirname $0))
cd $SCRIPT_DIR

rm -f task_additional_1_out

mpirun -np 2 ./task_additional_1 0 1     >> task_additional_1_out
mpirun -np 2 ./task_additional_1 0 10    >> task_additional_1_out
mpirun -np 2 ./task_additional_1 0 100   >> task_additional_1_out
mpirun -np 2 ./task_additional_1 0 1000  >> task_additional_1_out
mpirun -np 2 ./task_additional_1 0 10000 >> task_additional_1_out

mpirun -np 2 ./task_additional_1 1 1     >> task_additional_1_out
mpirun -np 2 ./task_additional_1 1 10    >> task_additional_1_out
mpirun -np 2 ./task_additional_1 1 100   >> task_additional_1_out
mpirun -np 2 ./task_additional_1 1 1000  >> task_additional_1_out
mpirun -np 2 ./task_additional_1 1 10000 >> task_additional_1_out

mpirun -np 2 ./task_additional_1 2 1     >> task_additional_1_out
mpirun -np 2 ./task_additional_1 2 10    >> task_additional_1_out
mpirun -np 2 ./task_additional_1 2 100   >> task_additional_1_out
mpirun -np 2 ./task_additional_1 2 1000  >> task_additional_1_out
mpirun -np 2 ./task_additional_1 2 10000 >> task_additional_1_out

mpirun -np 2 ./task_additional_1 3 1     >> task_additional_1_out
mpirun -np 2 ./task_additional_1 3 10    >> task_additional_1_out
mpirun -np 2 ./task_additional_1 3 100   >> task_additional_1_out
mpirun -np 2 ./task_additional_1 3 1000  >> task_additional_1_out
mpirun -np 2 ./task_additional_1 3 10000 >> task_additional_1_out
# mpirun -np 2 ./task_additional_1 3 100000 >> task_additional_1_out
# mpirun -np 2 ./task_additional_1 3 1000000 >> task_additional_1_out
# mpirun -np 2 ./task_additional_1 3 10000000 >> task_additional_1_out
# mpirun -np 2 ./task_additional_1 3 100000000 >> task_additional_1_out
