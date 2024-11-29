# IntroPARCO_H1
<p>To run the code on the Unitn HPC Cluster:</p>
<p>The code was run in gcc9.1 </p>
<p>Change the cd directive to the location where the trasnp.c file is saved.</p>
<p>Run the dos2unix transpose_h1.pbs command if on necessary</p>
<p>Run the .pbs file: qsub transpose_h1.pbs</p>
<p>It is possible to change the size of the matrix from the .pbs: it is the argument of the executale ./exe size</p>
<p>The code will run with 1, 2, 4, 8, 16, 32, 64 threads</p>
