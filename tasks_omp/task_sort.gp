#!/usr/bin/gnuplot
set terminal pngcairo size 1920,1080 font "Helvetica,30"

set xlabel "Depth" offset 0,0.5
set ylabel "Time, s" offset 1,0

set output "task_sort_1.png"

plot "task_sort.csv" with lines notitle
