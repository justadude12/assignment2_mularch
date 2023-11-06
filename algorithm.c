/*
============================================================================
Filename    : algorithm.c
Author      : Vincent VENTURA, Hana EL MOUTAOUKIL
SCIPER		: 302810, 340995
============================================================================
*/
#include <math.h>
#include <omp.h>

#define INPUT(I,J) input[(I)*length+(J)]
#define OUTPUT(I,J) output[(I)*length+(J)]


void simulate(double *input, double *output, int threads, int length, int iterations)
{
    double *temp;
    omp_set_num_threads(threads);
    #pragma omp parallel
    for(int n=0; n < iterations; n++)
    {
        #pragma omp for schedule(static)
        for(int i=1; i<length-1; i++)
        {
            for(int j=1; j<length-1; j+=2)
            {
                if ( ((i == length/2-1) || (i== length/2))
                    && ((j == length/2-1) || (j == length/2)) )
                        continue;
                if ( ((i == length/2-1) || (i== length/2))
                    && ((j+1 == length/2-1) || (j+1 == length/2)) )
                        continue;



                OUTPUT(i,j+1) = (INPUT(i-1,j) + INPUT(i-1,j+1) + INPUT(i-1,j+2) +
                                    INPUT(i,j)   + INPUT(i,j+1)   + INPUT(i,j+2)   +
                                    INPUT(i+1,j) + INPUT(i+1,j+1) + INPUT(i+1,j+2) )/9;

                OUTPUT(i,j) = (INPUT(i-1,j-1) + INPUT(i-1,j) + INPUT(i-1,j+1) +
                                    INPUT(i,j-1)   + INPUT(i,j)   + INPUT(i,j+1)   +
                                    INPUT(i+1,j-1) + INPUT(i+1,j) + INPUT(i+1,j+1) )/9;
            }
        }

        #pragma omp critical
        {
            temp = input;
            input = output;
            output = temp;
        }
    }
}