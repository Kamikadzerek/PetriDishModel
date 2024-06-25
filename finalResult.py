# %%
import os
from pygnuplot import gnuplot
from natsort import natsorted

# %%
types = ["DENSITY", "MRNLC", "SD"]
directory = os.getcwd()
files = {}
for t in types:
    files[t] = sorted(
        [
            f
            for f in os.listdir(directory)
            if (f.startswith(t) and f.endswith(".csv") and "avera" in f)
        ]
    )


# %%
for t in types:
    g = gnuplot.Gnuplot(log=False)
    g.cmd("set fit errorvariables")
    g.cmd('set print "' + t + '.dat"')
    if os.path.exists(t + ".dat"):
        os.remove(t + ".dat")
    for file in natsorted(files[t]):
        if t == "DENSITY":
            g.cmd(
                'fit a*x+b "' + file + '" u ($1)*2*' + file[7:11] + ":($2)*4/3 via a,b"
            )
        elif t == "MRNLC":
            g.cmd('fit a*x+b "' + file + '" u ($1)*pi*2*' + file[5:9] + ":($2) via a,b")
        else:
            g.cmd('fit a*x**b "' + file + '" u ($1):($2) via a,b')
        g.cmd("print " + file.split("_")[0][-4:] + ", a,b,a_err,b_err")
    g.close()
# %%
# %%
