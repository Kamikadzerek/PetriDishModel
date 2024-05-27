import numpy as np
f = open("jobs", "w")
for p in np.arange(5,15,1):
    for n in range(5):
        f.write("./PetriDishModel "+"{:.2f}".format(p)+" "+str(n)+" "+str(n+1)+"\n")
