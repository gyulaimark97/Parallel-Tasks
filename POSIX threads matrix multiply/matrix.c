
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>


//defining the matrix dimension and the nomber of threads
#define LENGHT 3
#define NUM_THREADS 8

 //global matrices
    int A[LENGHT][LENGHT]={{1, 2, 3},
                 {4, 5, 6},
                 {7, 8, 9}};

    int B[LENGHT][LENGHT]={{9, 8, 7},
                 {6, 5, 4},
                 {3, 2, 1}};
    int C[LENGHT][LENGHT];

//struct for threads
struct thread_data{
    int index_column;
    int index_row;
    int thread_id; //only for watching thread's number
};


struct thread_data thread_data_array[NUM_THREADS];


//function what calculate a value in the result matrix (result matrix now is C)
int dot_value(int A[LENGHT][LENGHT],int B[LENGHT][LENGHT],int index_row, int index_column){

    int dot=0;
    int i=0;

    for(;i<LENGHT;i++){
            dot=dot+A[i][index_column]*B[index_row][i];
    }

    return dot;
}


//thread's function for pthread
void *Thread (void* data){
    
    struct thread_data* mydata;
    mydata=(struct thread_data *)data; 
    printf("I am thread no. %d.\n", mydata->thread_id); //for watching threads

    //using dot value();
    int value=dot_value(A,B,mydata->index_row,mydata->index_column);
    //printf("\n%d\n", value);
    C[mydata->index_row][mydata->index_column]=value;


    
   //printf("%d, %d, %d \n",mydata->index_row,mydata->index_column, value );
}

int main (){

    pthread_t threads[NUM_THREADS];

    int i=0;
    int j=0;
    int k=0;
    int thread=0; 

        //calling threads with the right indexes
        for(;i<LENGHT;i++) {
            for(j=0;j<LENGHT;j++){

                    thread_data_array[k].index_column=i;
                    thread_data_array[k].index_row=j;
                    thread_data_array[k].thread_id=k;
                    thread=pthread_create(&threads[k], NULL, Thread,(void*) &thread_data_array[k]);
                    k++;
                    if(k==NUM_THREADS)
                        k=0;

            }
            
        }

        //waiting for all thread for right result 
        //without it some results wont be in the result matrix
        for(i=0;i<NUM_THREADS;i++){
            int result=pthread_join(threads[i],NULL);
            printf("Thread no. %d closed.\n", i);
            
        }
        

    //printing out the result matrix
        i=0;
        j=0;
        printf("\n");
       for(;i<LENGHT;i++) {
            for(j=0;j<LENGHT;j++){
                printf("%d ", C[i][j]);
                if(j==LENGHT-1)
                    printf("\n");
            }
        }

    return 0;
}
