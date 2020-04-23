#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include "lib/stopper.h"
#include "lib/stopper.c"

// it defines size of the arrays which contains the numbers where the program search for the maximum
#define SIZE 32768
#define B_SIZE 4  //log SIZE/2
#define NUM_OF_BITS 16

// the binary shape of the integers on NUM_OF_BITS bit
// its only has an int array which size is NUM_OF_BITS
struct binary
{
    int bits[NUM_OF_BITS];
};

//structure of a B_cell used for splitting a binary to bit arrays
struct B_cell
{
    int bits[B_SIZE];
};


//a method that initialize a binary number structure 
    //params:
// -a pointer to a binary type variable
//returns: 
    //nothing
void binary_init(struct binary *bin)
{
    int i;
    for ( i = 0; i < NUM_OF_BITS; i++)
    {
        bin->bits[i]=0;
    }
}


// a method for printing a binary to console
//params:
    // -it needs a binary
//returns:
    // nothing
void print_binary(struct binary bin)
{
    int i;
    for(i = NUM_OF_BITS-1; i>=0; i--)
        printf("%d",bin.bits[i]);
    printf("\n");
}


//method for reading the number of threads
//params:
    // argc : int
    // argv : char* array
//returns:
    // num_of_threads : int    the number of the threads
int get_arg(int argc, char* argv[])
{
    int num_of_threads;
    if(argc < 3)
    {
        perror("Adjon meg indítási paramétereket az aábbi módon : ./egesz {szalak száma : egész} {output fájl neve : string}");
        exit(-1);
    }
    else
    {
        num_of_threads=atol(argv[1]);
    }

    return num_of_threads;
}



// a method for generating random numbers to an array
// params:
    // random_array : int[]
    // size : int the size of the random_array
// returns :
    //nothing
void random_generate(int random_array[], int size)
{
    srand(time(0));
    int i;

    for(i = 0; i < SIZE; i++)
    {
        random_array[i] = (rand()%32768);
    }
}

// a method for converting integers to struct binary
// params:
    // - n : int the number we want to convert to binary
    // bin : struct binary * the binary where the converted int goes
//returns:
    //nothing
void to_binary(int n, struct binary *bin) 
{
    int rem, i;
    int array[NUM_OF_BITS];
    int counter;

    //initializing array
    for(i =0; i<NUM_OF_BITS; i++)
        array[i]=0;

    //filling array with the bits of binary number
    i=0;
    while (n != 0) 
    {
        rem = n % 2;
        n /= 2;
        array[i] = rem;
        i++;
    }

    //filling up bin with the bits backward for the right representation 
    counter=i;
    int j=i;
    for (i = counter; i >= 0; i--)
    {
        bin->bits[i]=array[i];
    }

    

}

int back_to_the_integers(struct B_cell B, int size)
{
    int sum=0;
    int i;
    for ( i = size-1; i >=0 ; i--)
    {
        sum = sum + ( B.bits[i]*pow(2,i) );
    }
    return sum;
    
}

stopperOMP stopper_clock;

//main
int main(int argc, char* argv[]){
//variables
int num_of_threads; //numbers of threads
int random_array[SIZE]; // the array of random numbers
int indexes[SIZE]; //index array, in algorithm: RS
int i; //for loops
int j;
int k;
int const c=2; // c from the algorithm
struct binary binary_numbers[SIZE]; //binary numbers array
struct B_cell B[SIZE][2*c]; // B matrix
//getting the maximum
int max=0;
int temp=0;


//starting the stopper 
startSOMP(&stopper_clock);

FILE* file;
file=fopen(argv[2],"a+");


//getting the number of threads
num_of_threads=get_arg(argc, argv);

//generating the random numbers
random_generate(random_array, SIZE);



// filling up the index and binary arrays
for(i = 0; i < SIZE; i++)
{
    indexes[i]=i+1;
    binary_init(&binary_numbers[i]);
    to_binary(random_array[i], &binary_numbers[i]);
}

// making the B matrix
for(i = 0; i < SIZE; i++)
{
    int remaining_bits=NUM_OF_BITS-1;
    for(j = 0; j < (2*c); j++)
    {
        for ( k = B_SIZE-1; k>=0; k--)
        {
            B[i][j].bits[k]=binary_numbers[i].bits[remaining_bits];
            remaining_bits--;

        }
        
    }   
}



//iterating over the columns
for(j = 0; j < (2*c); j++)
{
    max=0;
    
    #pragma parallel shared(idexes, temp, max ) private(i) 
        #pragma parallel for 
    // first loop: searching for the maximum value where the index is not 0
    // second loop making the indexes zeroes where the value is lower then max 
    for(i = 0; i < SIZE; i++)
    {
        temp=back_to_the_integers(B[i][j], B_SIZE);

        if(temp > max && indexes[i]!=0){
            max=temp;
        }
    }
    for(i = 0; i < SIZE; i++)
    {
        temp=back_to_the_integers(B[i][j], B_SIZE);

        if(temp < max && indexes[i]!=0){
            indexes[i]=0;
        }
    }
}

stopSOMP(&stopper_clock);



// for debug reasons
/*for(i = 0; i < SIZE; i++)
{

    if (indexes[i]!=0)
    {
        printf("%d  ",indexes[i]);
        printf("  %d  ",random_array[i]);
        print_binary(binary_numbers[i]);
    }
}*/


double time=tprintfOMP(&stopper_clock, "%lf");
fprintf(file,"%02lf\n",time);

return 0;
}