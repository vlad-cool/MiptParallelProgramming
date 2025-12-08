#!/usr/bin/gnuplot

reset session
set terminal pngcairo size 1200,800 enhanced font 'Verdana,10'
set output "plots/1.png"

datafile = "results/omp_100"

set xlabel "x"
set ylabel "y"
set grid

plot datafile using 1:2 with lines lw 2 lc rgb "red" notitle
