#!/bin/sh

PRECISION=3

./bin/main -t 1 -p $PRECISION
./bin/main -t 2 -p $PRECISION
./bin/main -t 4 -p $PRECISION
./bin/main -t 8 -p $PRECISION
