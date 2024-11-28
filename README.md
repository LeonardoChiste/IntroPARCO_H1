# IntroPARCO_H1
<p>To run the code on the Unitn HPC Cluster:</p>
<p>Change the cd directive to the location where the trasnp.c file is saved.</p>
<p>Run the dos2unix transpose_h1.pbs command if on Windows</p>
<p>Run the .pbs file: qsub transpose_h1.pbs</p>
<p>It is possible to change the size of the matrix from the .pbs: it is the argument of the executale ./exe size</p>
<p>The code will run each symulaton 5 times, to provide a more realistic look into the performance</p>
