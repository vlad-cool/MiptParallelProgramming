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

WIDTH=8192
HEIGHT=256
STEPS=256
SEED=179

SCRIPT_DIR=$(realpath $(dirname $0))

HASH=-H
INFO=-i

echo $(WIDTH)x$(HEIGHT), $STEPS steps

mpirun -np 1 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m SINGLE

mpirun -np 1 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m SHARED -p 1
mpirun -np 1 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m SHARED -p 2
mpirun -np 1 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m SHARED -p 4
mpirun -np 1 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m SHARED -p 8
mpirun -np 1 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m SHARED -p 12

mpirun -np 1 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m MPI 
mpirun -np 2 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m MPI
mpirun -np 4 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m MPI
mpirun -np 8 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m MPI

mpirun -np 1 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m MPI -o
mpirun -np 2 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m MPI -o
mpirun -np 4 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m MPI -o
mpirun -np 8 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m MPI -o

mpirun -np 8 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m HYBRID -p 2 -o
mpirun -np 8 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m HYBRID -p 2
mpirun -np 4 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m HYBRID -p 4 -o
mpirun -np 4 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m HYBRID -p 4
mpirun -np 2 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m HYBRID -p 8 -o
mpirun -np 2 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m HYBRID -p 8
