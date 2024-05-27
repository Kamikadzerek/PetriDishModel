set grid
set terminal png size 1000,800 enhanced font "Helvetica,20"
set output "plot.png"
set key right bottom
set ylabel '{/symbol r}_v'
set xlabel 't - grubość wyrażona w średnicach komórek'
set xrange [1:15]
set xtics 1
#set yrange [0.6:0.91]
#set logscale x
plot \
"densities.dat" u ($1):($2) pt 5 ps 0.2 lc rgb '#000000' notitle,\


