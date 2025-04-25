#!/bin/sh

SCRIPT_DIR=$(realpath $(dirname $0))
NUMBERS=8192

mpirun -np 1  $SCRIPT_DIR/bin/main $NUMBERS
mpirun -np 2  $SCRIPT_DIR/bin/main $NUMBERS
mpirun -np 3  $SCRIPT_DIR/bin/main $NUMBERS
mpirun -np 4  $SCRIPT_DIR/bin/main $NUMBERS
mpirun -np 6  $SCRIPT_DIR/bin/main $NUMBERS
mpirun -np 8  $SCRIPT_DIR/bin/main $NUMBERS
mpirun -np 12 $SCRIPT_DIR/bin/main $NUMBERS
mpirun -np 16 $SCRIPT_DIR/bin/main $NUMBERS

# mpirun -np 1 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m SINGLE

# mpirun -np 1 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m SHARED -p 1
# mpirun -np 1 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m SHARED -p 2
# mpirun -np 1 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m SHARED -p 4
# mpirun -np 1 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m SHARED -p 8
# mpirun -np 1 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m SHARED -p 12

# mpirun -np 1 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m MPI 
# mpirun -np 2 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m MPI
# mpirun -np 4 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m MPI
# mpirun -np 8 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m MPI

# mpirun -np 1 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m MPI -o
# mpirun -np 2 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m MPI -o
# mpirun -np 4 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m MPI -o
# mpirun -np 8 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m MPI -o

mpirun -np 8 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m HYBRID -p 2 -o
mpirun -np 8 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m HYBRID -p 2
# mpirun -np 4 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m HYBRID -p 4 -o
# mpirun -np 4 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m HYBRID -p 4
# mpirun -np 2 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m HYBRID -p 8 -o
# mpirun -np 2 $SCRIPT_DIR/bin/main -w $WIDTH -h $HEIGHT -s $STEPS $HASH -t $INFO -r $SEED -m HYBRID -p 8
