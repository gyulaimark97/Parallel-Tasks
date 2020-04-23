#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lib/stopper.h"
#include "lib/stopper.c"

stopperOMP stopper_clock;


int dot (int* row, int* column, int dimension){
    int i;
    int result=0;
    for (i = 0; i < dimension; i++)
    {
        result=result+(row[i]*column[i]);
    }
    
    return result;
}




int main (int argc, char* argv[]){

    startSOMP(&stopper_clock);

	srand(time(0));

    FILE* file;

    if(argc<2)
    {
        perror("adja meg egy fájlnevet ahova az eredmény kiíródik");
        exit(-1);
    }
    else
    {
        file=fopen(argv[1],"a+");
    }
    


    int dimension= 1024;
    int i=0;
    int j=0;
    int sum=0;


    int **A = malloc(dimension * sizeof(int *));
    int **B = malloc(dimension * sizeof(int *));
    int **C = malloc(dimension * sizeof(int *));
	for(i = 0; i < dimension; i++){
		A[i] = malloc(dimension * sizeof(int));
        B[i] = malloc(dimension * sizeof(int));
        C[i] = malloc(dimension * sizeof(int));
    }



    for (i = 0; i < dimension; i++)
    {
        for (j = 0; j < dimension; j++)
        {
            A[i][j]=rand()%100+1;
            B[i][j]=rand()%100+1;
            C[i][j]=1;
        }
        
    }



    for (i = 0; i < dimension; i++)
    {
        for (j = 0; j < dimension; j++)
        {
            C[i][j]=dot(A[i], B[j], dimension);
        }
        
    }




    free(A);
    free(B);
    free(C);
    stopSOMP(&stopper_clock);
    double time=tprintfOMP(&stopper_clock, "%lf");

    fprintf(file,"%02lf\n",time);

    fclose(file);


    return 0;
}
