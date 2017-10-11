#!/usr/bin/gnuplot
reset

# svg
set terminal svg dynamic fname 'Helvetica, Verdana' \
fsize '14'
set output '${svg_filename}'

# color definitions
set border linewidth 1.5
set style line 1 lc rgb '#62a0ca' lt 1 lw 2 pt 7 ps 1.5 # --- blue
set style line 2 lc rgb '#e26868' lt 1 lw 2 pt 5 ps 1.5 # --- red
set style line 3 lc rgb '#ffa556' lt 1 lw 2 pt 5 ps 1.5 # --- yellow
set style line 4 lc rgb '#6bbc6b' lt 1 lw 2 pt 5 ps 1.5 # --- green
set style line 5 lc rgb '#b495d1' lt 1 lw 2 pt 5 ps 1.5 # --- purple

set key default

set xtics 1

set title "${title}"

set xlabel "${xlabel}"
set ylabel "${ylabel}"

set label "${label}" at graph 0.15, graph 0.95 font 'Helvetica Bold'

plot '${datafile}' index 0 title 'Bit array' with linespoints ls 1, \
''                      index 1 title 'Bool array' with linespoints ls 2
