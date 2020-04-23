#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include "lib/stopper.h"
#include "lib/stopper.c"


//defining the matrix dimension and the nomber of threads
#define LENGHT 800


int A [LENGHT][LENGHT];
int B [LENGHT][LENGHT];
int C [LENGHT][LENGHT];

struct stopperOMP stopper_clock;


struct dat {
   int i; /* row */
   int j; /* column */
};


void *thread_func(void *param) {
   struct dat *data = param; // the structure that holds our data
   int n, sum = 0; //the counter and sum

   //Row multiplied by column
    for(n = 0; n< LENGHT; n++){
      sum += A[data->i][n] * B[n][data->j];
    }
   //assign the sum to its coordinate
    C[data->i][data->j] = sum;

   //Exit the thread
    pthread_exit(0);
}



int main (int argc, char* argv[]){

    startSOMP(&stopper_clock);
    FILE* file;
    int i,j, count = 0;

    if(argc<2)
    {
        perror("adja meg a szálak számát és egy fájlnevet ahova az eredmény kiíródik");
        exit(-1);
    }
    else
    {
        file=fopen(argv[1],"a+");
    }


    for ( i = 0; i < LENGHT; i++)
    {
        for ( j = 0; j < LENGHT; j++)
            {
                A[i][j]=(rand()%2000)+1;
                B[i][j]=(rand()%2000)+1;
                C[i][j]=0;
            }
                
    }   

    for(i = 0; i < LENGHT; i++) 
    {
        for(j = 0; j < LENGHT; j++) 
        {
        struct dat *data = (struct dat *) malloc(sizeof(struct dat));
        data->i = i;
        data->j = j;


        pthread_t tid; 
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tid,&attr,thread_func,data);
        pthread_join(tid, NULL);
        }
    }

        stopSOMP(&stopper_clock);
        double time =tprintfOMP(&stopper_clock, "%lf");
        fprintf(file, "%lf\n", time);
        fclose(file);



    return 0;
}
