#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "lib/stopper.h"
#include "lib/stopper.c"




struct stopperOMP stopper_clock;

int main(int argc, char* argv[]){

    startSOMP(&stopper_clock);

int i;
int j;
int k;
int dimension = 800; //800 
int num_of_thread;
FILE* file;

    if(argc<2)
    {
        perror("adja meg a szálak számát és egy fájlnevet ahova az eredmény kiíródik");
        exit(-1);
    }
    else
    {
        num_of_thread=atol(argv[1]);
        file=fopen(argv[2],"a+");
    }

omp_set_num_threads(num_of_thread);


int A[ dimension ] [ dimension ];
int B[ dimension ] [ dimension ];
int C[ dimension ] [ dimension ];



    for ( i = 0; i < dimension; i++)
    {
        for ( j = 0; j < dimension; j++)
            {
                A[i][j]=(rand()%2000)+1;
                B[i][j]=(rand()%2000)+1;
                C[i][j]=0;
            }
                
    }


    #pragma omp parallel shared(A,B,C) private(i,j,k) 
    {
        #pragma omp for schedule(static)
        for (i=0; i<dimension; i=i+1){

            for (j=0; j<dimension; j=j+1){

                for (k=0; k<dimension; k=k+1){

                    C[i][j]=(C[i][j])+((A[i][k])*(B[k][j]));

                }

            }

        }
    }
    /* for writing the result to the console
    printf("C:\n"); 
    for ( i = 0; i < dimension; i++)
    {
        for ( j = 0; j < dimension; j++)
        {
            printf("%d ", C[i][j]);
        }

        printf("\n");       
    }
    */
    
    stopSOMP(&stopper_clock);

    double time=tprintfOMP(&stopper_clock, "%lf");

    fprintf(file, "%lf\n", time);
    fclose(file);
}