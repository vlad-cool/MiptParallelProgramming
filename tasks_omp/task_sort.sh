#!/bin/sh
cd "$(dirname "$0")"

exec > >(tee task_sort.csv)

for i in {-1..32}; do 
    echo -n "$i, "
    ./task_sort_1 4000000 $i | grep -oP '\d+\.\d+(?= seconds)'
done

./task_sort.gp
