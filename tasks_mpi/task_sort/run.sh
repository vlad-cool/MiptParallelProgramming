#!/bin/sh

SCRIPT_DIR=$(realpath $(dirname $0))
NUMBERS=10000

mpirun -np 1  $SCRIPT_DIR/bin/main $NUMBERS
mpirun -np 2  $SCRIPT_DIR/bin/main $NUMBERS
mpirun -np 3  $SCRIPT_DIR/bin/main $NUMBERS
mpirun -np 4  $SCRIPT_DIR/bin/main $NUMBERS
mpirun -np 6  $SCRIPT_DIR/bin/main $NUMBERS
mpirun -np 8  $SCRIPT_DIR/bin/main $NUMBERS
mpirun -np 12 $SCRIPT_DIR/bin/main $NUMBERS
mpirun -np 16 $SCRIPT_DIR/bin/main $NUMBERS

NUMBERS=200000

mpirun -np 1  $SCRIPT_DIR/bin/main $NUMBERS
mpirun -np 2  $SCRIPT_DIR/bin/main $NUMBERS
mpirun -np 3  $SCRIPT_DIR/bin/main $NUMBERS
mpirun -np 4  $SCRIPT_DIR/bin/main $NUMBERS
mpirun -np 6  $SCRIPT_DIR/bin/main $NUMBERS
mpirun -np 8  $SCRIPT_DIR/bin/main $NUMBERS
mpirun -np 12 $SCRIPT_DIR/bin/main $NUMBERS
mpirun -np 16 $SCRIPT_DIR/bin/main $NUMBERS
