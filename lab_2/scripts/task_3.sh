#!/bin/bash

cd $(dirname $0)/..

echo "base:"
bin/task_3_base data/task_3_base
echo "omp"
bin/task_3_omp data/task_3_omp

diff data/task_3_base data/task_3_omp
