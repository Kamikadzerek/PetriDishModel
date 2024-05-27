import os
from pygnuplot import gnuplot
from natsort import natsorted
directory = os.getcwd()
files = sorted([f for f in os.listdir(directory) if (f.startswith('DENSITY') and f.endswith('.csv') and "avera" in f)])
g =gnuplot.Gnuplot(log=True)
g.cmd("set fit errorvariables")
g.cmd('set print "densities.dat"')
if os.path.exists("densities.dat"):
  os.remove("densities.dat")
for file in natsorted(files):
    g.cmd('a=0')
    g.cmd('fit a*x "'+file+'" u ($1)*2*'+file[7:11]+':($2)*4/3 via a')
    g.cmd('print '+file[7:11]+', a,a_err')
g.cmd("set xtics 0.5")
g.cmd("set grid")
#g.cmd('plot "densities.dat" u 1:2 pt 5 ps 1.0 lc rgb "#000000" notitle, "densities.dat" u 1:2 with line pt 5 ps 1.0 lc rgb "#000000" notitle')
