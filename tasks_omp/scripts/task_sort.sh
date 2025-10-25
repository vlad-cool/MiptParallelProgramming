#!/bin/sh
cd "$(dirname "$0")"

rm -f ../results/task_sort_1.csv ../results/task_sort_2.csv ../results/task_sort_3.csv

for i in {0..16}; do 
    echo -n "$i, " >> task_sort_1.csv
    ../bin/task_sort_1 4000000 $i | grep -oP '\d+\.\d+(?= seconds)' >> ../results/task_sort_1.csv
done

for i in {8..16}; do 
    echo -n "$i, " >> task_sort_2.csv
    ../bin/task_sort_1 4000000 $i a | grep -oP '\d+\.\d+(?= seconds)' >> ../results/ask_sort_2.csv
done

./bin/task_sort.gp
