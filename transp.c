#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#ifdef _OPENMP
	#include <omp.h>
#endif

void freeMatrix(double **M, int n) {
	for (int i = 0; i < n; i++) {
    	free(M[i]);
	}
	free(M);
}
int checkSym(double **M, int n){
   int sym=1;
   for(int i=0; i<n; i++){
    	for(int j=0; j<n; j++){
        	if(M[i][j]!=M[j][i])
           	sym = 0;
    	}
   }
   return sym;
}
void matTranspose(double **M, double **T, int n){
   for (int i = 0; i < n; ++i){
    	for (int j = 0; j < n; ++j) {
         	T[i][j] = M[j][i];
    	}
   }
}
int checkSymTiling(double **M, int n){
	int sym=1;
    int jump=8;
    for (int i = 0; i < n; i+=jump){
    	for (int j = 0; j < n; j+=jump) {
         	for (int k = i; k < i+jump; ++k){
                for (int h = j; h < j+jump; ++h) {
                    if(M[k][h]!=M[h][k])
           	        sym = 0;
                }
         	}
    	}
    }
    return sym;
}
void matTransposeTiling(double **M, double **T, int n){
   int jump=8;
   for (int i = 0; i < n; i+=jump){
    	for (int j = 0; j < n; j+=jump) {
         	for (int k = i; k < i+jump; ++k){
    	         for (int h = j; h < j+jump; ++h) {
                   	T[k][h] = M[h][k];
                   	}
         	}
    	}
   }
}
int checkSymImp(double **M, int n){
   int sym=1;
   #pragma simd
   for(int i=0; i<n; i++){
    	#pragma unroll(4)  
    	for(int j=0; j<n; j++){
        	if(M[i][j]!=M[j][i])
           	sym = 0;
    	}
   }
   return sym;
}
void matTransposeImp(double **M, double **T, int n){
   #pragma simd
   for (int i = 0; i < n; ++i){
    	#pragma unroll(4)
    	for (int j = 0; j < n; ++j) {
         	T[i][j] = M[j][i];
    	}
   }
}
int checkSymOMP(double **M, int t, int n){
   int sym=1;
#pragma omp parallel num_threads(t) 
{
   #pragma omp for collapse(2) schedule(static, 8)
   for(int i=0; i<n; i++){
    	for(int j=0; j<n; j++){
        	if(M[i][j]!=M[j][i])
           	sym = 0;
    	}
   }
}
   return sym;
}
void matTransposeOMP(double **M, double **T, int t, int n){
#pragma omp parallel num_threads(t) 
{
   #pragma omp for collapse(2) schedule(static, 8)
   for (int i = 0; i < n; ++i){
    	for (int j = 0; j < n; ++j) {
         	T[i][j] = M[j][i];
    	}
   }
}
}
int main(int argc, char *argv[]) {
   double **M;
   double **T;
   int symmetric, repeat=1, n;
#ifdef _OPENMP
   double wt1,wt2;
#endif
   if(argc !=2 ){
      printf("Error: wrong number of parameters");
   }
   else{
    n=atoi(argv[1]);
   	printf("Size: %d x %d\n", n, n);
   	M = (double **)malloc(n * sizeof(double *));
   	T = (double **)malloc(n * sizeof(double *));
   	for (int i = 0; i < n; i++) {
        	M[i] = (double *)malloc(n * sizeof(double));
   	}
   	for (int i = 0; i < n; i++) {
        	T[i] = (double *)malloc(n * sizeof(double));
   	}
   	for (int i = 0; i < n; i++) {
        	for (int j = 0; j < n; j++) {
         	M[i][j] = (double)(rand()%10000)/100.0;
        	}
   	}
   
   	printf("sequential: ");
   	for(int t=0; t<repeat; t++){
    	#ifdef _OPENMP
       	wt1=omp_get_wtime();
    	#endif
       	symmetric = checkSym(M, n);
       	matTranspose(M, T, n);
    	#ifdef _OPENMP
       	wt2=omp_get_wtime();
    	#endif  
    	#ifdef _OPENMP
       	printf( "%2.6g, ", wt2-wt1 );
    	#endif
   	}

   	printf("\ntiling: ");
   	for(int t=0; t<repeat; t++){
    	#ifdef _OPENMP
       	wt1=omp_get_wtime();
    	#endif
       	symmetric = checkSymTiling(M, n);
       	matTransposeTiling(M, T, n);
    	#ifdef _OPENMP
       	wt2=omp_get_wtime();
    	#endif  
    	#ifdef _OPENMP
       	printf( "%2.6g, ", wt2-wt1 );
    	#endif
   	}
  	 
   	printf("\nsimd: ");
   	for(int t=0; t<repeat; t++){
    	#ifdef _OPENMP
       	wt1=omp_get_wtime();
    	#endif
       	symmetric = checkSymImp(M, n);
       	matTransposeImp(M, T, n);
    	#ifdef _OPENMP
       	wt2=omp_get_wtime();
    	#endif  
    	#ifdef _OPENMP
       	printf( "%2.6g, ", wt2-wt1 );
    	#endif
   	}

   	printf("\nOMP code:\n");
   	for(int num_threads=1; num_threads<=64; num_threads*=2){
       	printf("threads: %d\n", num_threads);
       	for(int t=0; t<repeat; t++){
        	#ifdef _OPENMP
          	wt1=omp_get_wtime();
        	#endif
        	#ifdef _OPENMP
          	symmetric = checkSymOMP(M, num_threads, n);
          	matTransposeOMP(M, T, num_threads, n);
        	#endif
        	#ifdef _OPENMP
          	wt2=omp_get_wtime();
        	#endif
        	#ifdef _OPENMP
           	printf("%2.6g, ", wt2-wt1);
        	#endif
       	}
       	printf("\nsymmetric: %d\n", symmetric);
  	}
  	//checks if the transpose was computed
  	int c=1;
  	printf("check:");
  	for(int i=0; i<n; i++){
       	for(int j=0; j<n; j++){
        	if(M[i][j]!=T[j][i])
           	c = 0;
       	}
  	}
  	printf("%d\n\n", c);
   
   
  	freeMatrix(M, n);
  	freeMatrix(T, n);
   }
   
   
   return 0;
}




