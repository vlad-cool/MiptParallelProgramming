#!/usr/bin/gnuplot
set terminal pngcairo size 1920,1080 font "Helvetica,30"
set output "result/heatmap.png"

# set palette defined (0 "blue", 1 "red")
set palette defined (0 "black", 1 "white")
set pm3d map

set xlabel "X Axis Name" offset 0,0.5
set ylabel "Y Axis Name" offset 1,0
set xtics 2.5
set ytics 2.5

stats "result/data.dat" matrix nooutput
x_scale = 10.0/(STATS_size_x-1)
y_scale = 10.0/(STATS_size_y-1)

splot "result/data.dat" matrix using ($1*x_scale):($2*y_scale):3 with image notitle
