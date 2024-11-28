# IntroPARCO_H1
To run the code on the Unitn HPC Cluster:
Change the cd directive to the location where the trasnp.c file is saved.
Run the dos2unix transpose_h1.pbs command if on Windows
Run the .pbs file: qsub transpose_h1.pbs
It is possible to change the size of the matrix from the .pbs: it is the argument of the executale ./exe size
The code will run each symulaton 5 times, to provide a more realistic look into the performance
