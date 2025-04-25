#!/bin/sh

# -w WIDTH
# -h HIGH
# -s STEPS
# -m {SINGLE, SHARED, MPI, HYBRID}
# -p {NUMBER_OF_THREADS} (for shared and hybrid (per mpi process))
# -o (optimize mpi send)
# -a (print all steps)
# -H (print hash)
# -f (print field)
# -t (print calculation time)
# -i (print launch info)
# -d DELAY (in milliseconds, make sense with -a -f options to make good animation)
# -r SEED (seed for random, default random_device())
# -P PERCENT (percentage for random fill)

WIDTH=10000000
HEIGHT=16
STEPS=8
SEED=179

SCRIPT_DIR=$(realpath $(dirname $0))
# cd $SCRIPT_DIR
# SCRIPT_DIR=/students/rt20250309/parallel_programming/tasks/task_gol
# SCRIPT_DIR=.

HASH=-H
INFO=-i

echo $WIDTH x $HEIGHT, $STEPS steps

mpirun -np 1 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -P 1 -m MPI 
mpirun -np 2 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -P 1 -m MPI
mpirun -np 4 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -P 1 -m MPI
mpirun -np 8 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -P 1 -m MPI

mpirun -np 1 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -P 1 -m MPI -o
mpirun -np 2 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -P 1 -m MPI -o
mpirun -np 4 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -P 1 -m MPI -o
mpirun -np 8 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -P 1 -m MPI -o
