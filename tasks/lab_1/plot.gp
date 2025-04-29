#!/usr/bin/gnuplot
set terminal pngcairo size 800,600
set output "heatmap.png"

set palette defined (0 "blue", 1 "red")
set pm3d map
splot "data.dat" matrix with image
