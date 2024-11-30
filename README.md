# IntroPARCO_H1
<p>To run the code on the Unitn HPC Cluster:</p>
<p>The code was run in gcc9.1 </p>
<p>Change the cd directive to the location where the trasnp.c file is saved.</p>
<p>Run the dos2unix transpose_h1.pbs command if on necessary</p>
<p>Run the .pbs file: qsub transpose_h1.pbs</p>
<p>It is possible to change the size of the matrix from the .pbs: it is the argument of the executale ./exe size</p>
<p>The code will run with 1, 2, 4, 8, 16, 32, 64 threads</p>
<p>To test the chache hit/miss:</p>
<p>The compare_seq_imp.c file allows to select either the sequential(1), loop tiling(2) or simd unroll(3) versions to measure the cache hit/miss with tools such as perf or likwid:</p>
<p>gcc -o comp compare_seq_imp.c -fopenmp   </p>
<p>perf stat -e cache - references , cache - misses ./comp 1</p>
<p>Loading the tools modules is necessary, this test was run in an interactive session, the results are not displaied if run on .pbs. The number next to the execurtable corresponds to: 1=sequential, 2=loop tiling, 3=simd</p>
