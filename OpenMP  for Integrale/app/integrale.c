
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include "lib/stopper.h"
#include "lib/stopper.c"




double global_sum=0;
int s;
double const INTEGRAL_LOWER_BOUND= 0;
double const INTEGRAL_UPPER_BOUND= 3000;
double const NUM_OF_POINTS= 10000;
double const NUM_OF_SUBPOINTS= 3000;

omp_lock_t writelock;

struct stopperOMP stopper_clock;




// the function which integrale we looking for
double f(double t){
    return t+sin(t)*9*t;
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

    omp_set_lock(&writelock);
    
        global_sum+=local_sum;

    omp_unset_lock(&writelock);

} 

int main (int argc, char* argv[]){

    startSOMP(&stopper_clock);
    FILE* file;
    int num_of_threads;

    if(argc<3)
    {
        perror("adja meg a szálak számát és egy fájlnevet ahova az eredmény kiíródik");
        exit(-1);
    }
    else
    {
        file=fopen(argv[2],"a+");
        num_of_threads=strtoll(argv[1], NULL, 10);
    }

    omp_set_num_threads(num_of_threads);

    //indexes
    int i;
    //other variables
    double answer=0;
    double distance =(INTEGRAL_UPPER_BOUND - INTEGRAL_LOWER_BOUND) / (NUM_OF_POINTS * NUM_OF_SUBPOINTS); // definating distance;
    omp_init_lock(&writelock);



    #pragma parallel shared(distance, writelock) private(i) 
        #pragma parallel for 
            for(i=0; i<NUM_OF_POINTS-1;i++){
                
                integrale(i,distance);

            }


    answer=( global_sum + distance )/  (2 * ( f(INTEGRAL_LOWER_BOUND) + f(INTEGRAL_UPPER_BOUND) )); 
    
        
    omp_destroy_lock(&writelock);

    stopSOMP(&stopper_clock);
    double time=tprintfOMP(&stopper_clock, "%lf");
    fprintf(file,"%lf\n", time);

    fclose(file);
    

    return 0;
}
