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


int main(int argc, char *argv[]) {
   if(argc != 2){
      printf("specify the code to run: 1=sequential, 2=loop-tiling, 3=simd unroll");
   }else{
      double **M;
      double **T;
      int symmetric, repeat=3;
   #ifdef _OPENMP
      double wt1,wt2;
   #endif
      switch(atoi(argv[1])){
          case 1:
              for(int n=16; n<=4096; n*=2){
   	          printf("\nSize: %d x %d  ", n, n);
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
    
              printf("sequential: \n");
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
              freeMatrix(M, n);
              freeMatrix(T, n);
              }
   	          
          break;
          case 2:
              for(int n=16; n<=4096; n*=2){
   	          printf("\nSize: %d x %d  ", n, n);
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
    
              printf("Loop tiling: \n");
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
              freeMatrix(M, n);
              freeMatrix(T, n);
              }
          break;
          case 3:
              for(int n=16; n<=4096; n*=2){
   	          printf("\nSize: %d x %d  ", n, n);
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
    
              printf("simd unroll: \n");
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
              freeMatrix(M, n);
              freeMatrix(T, n);
              }
          break;
          default: 
          printf("no such case"); 
          break;
      }
   }
   printf("\n");

return 0;
}
