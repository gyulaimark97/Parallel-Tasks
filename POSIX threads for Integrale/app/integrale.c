
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "lib/stopper.h"
#include "lib/stopper.c"




double global_sum=0;
int s;
double const INTEGRAL_LOWER_BOUND= 0;
double const INTEGRAL_UPPER_BOUND= 3000;
double const NUM_OF_POINTS= 10000;
double const NUM_OF_SUBPOINTS= 3000;
pthread_mutex_t mutex;


struct stopperOMP stopper_clock;



//struct for threads
struct thread_data{
    double distance;
    int index;
    int thread_id;  //only for watching thread's number

};

// the function which integrale we looking for
double f(double t){
    return t+sin(t);
}



// the function that execute the integration
void integrale(int index, double distance){

    double local_sum=0;
    double t=0;
    int i=0;

    t = (INTEGRAL_LOWER_BOUND+index*(INTEGRAL_UPPER_BOUND-INTEGRAL_LOWER_BOUND)/NUM_OF_POINTS);

    for(i = 0; i < NUM_OF_SUBPOINTS; i++){

        local_sum += f(t);
        t=t+distance;
    }

    local_sum *= distance;

    pthread_mutex_lock(&mutex);
    
        global_sum+=local_sum;

    pthread_mutex_unlock(&mutex);

} 


//thread's function for pthread
void *function_of_thread (void* data){
    
    struct thread_data* mydata;
    mydata=(struct thread_data *)data; 
    //printf("I am thread no. %d.\n", mydata->thread_id); //for watching threads
    
    integrale(mydata->index, mydata->distance);

}

int main (int argc, char* argv[]){

    startSOMP(&stopper_clock);
    int num_of_threads;
    FILE * file;

    if(argc<3)
    {
        perror("Adja meg a szálak számát és az output fájl nevét!");
        exit(-1);
    }
    else
    {
        num_of_threads=atol(argv[1]);
        file=fopen(argv[2], "a+");

    }
    

    pthread_t threads[num_of_threads];
    int thread=0;

    // thread date array
    struct thread_data thread_data_array[num_of_threads];

    //indexes
    int i;
    int j=0;

    //other variables
    double answer=0;
    double distance =(INTEGRAL_UPPER_BOUND - INTEGRAL_LOWER_BOUND) / (NUM_OF_POINTS * NUM_OF_SUBPOINTS); // definating distance;


    
    for(i=0; i<NUM_OF_POINTS-1;i++){
        
        thread_data_array[j].index=i;
        thread_data_array[j].distance=distance;
        thread_data_array[j].thread_id=j;
        thread=pthread_create(&threads[j],NULL, &function_of_thread, &thread_data_array[j]);

        //stepping on threads
        j++;
        if(j==num_of_threads)
            j=0;

    }



    //waiting for all thread for right result 
    //without it some results wont be in the result matrix
    for(i=0;i<num_of_threads;i++){
        int result=pthread_join(threads[i],NULL);
        //printf("Thread no. %d closed.\n", i);
            
    }

    answer=( global_sum + distance )/  (2 * ( f(INTEGRAL_LOWER_BOUND) + f(INTEGRAL_UPPER_BOUND) )); 
    //printf("The integrale is: %lf\n", answer);
        
    stopSOMP(&stopper_clock);
    double time = tprintfOMP(&stopper_clock, "%lf");

    fprintf(file, "%lf\n", time);

    fclose(file);

    return 0;
}
