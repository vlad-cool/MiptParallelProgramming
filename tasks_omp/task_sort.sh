#!/bin/sh
cd "$(dirname "$0")"

exec > >(tee task_sort.csv)

./task_sort_1 4000000 -1 > /dev/null 2> /dev/null
./task_sort_1 4000000 -1 > /dev/null 2> /dev/null
./task_sort_1 4000000 -1 > /dev/null 2> /dev/null
./task_sort_1 4000000 -1 > /dev/null 2> /dev/null

rm -f task_sort_1.csv task_sort_2.csv task_sort_3.csv

for i in {-1..24}; do 
    echo -n "$i, " >> task_sort_1.csv
    ./task_sort_1 4000000 $i | grep -oP '\d+\.\d+(?= seconds)' >> task_sort_1.csv
    echo -n "$i, " >> task_sort_2.csv
    ./task_sort_1 4000000 $i a | grep -oP '\d+\.\d+(?= seconds)' >> task_sort_2.csv
    # echo -n "$i, " >> task_sort_3.csv
    # ./task_sort_1 4000000 $i 4 | grep -oP '\d+\.\d+(?= seconds)' >> task_sort_3.csv
done

./task_sort.gp
