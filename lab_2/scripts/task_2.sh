#!/bin/bash

cd $(dirname $0)/..

echo "base:"
bin/task_2_base data/task_2_base
echo "omp"
bin/task_2_omp data/task_2_omp

diff data/task_2_base data/task_2_omp
