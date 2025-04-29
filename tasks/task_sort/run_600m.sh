#!/bin/sh

SCRIPT_DIR=$(realpath $(dirname $0))
NUMBERS=600000000

mpirun -np 8 $SCRIPT_DIR/bin/main $NUMBERS
