/*
============================================================================
Filename    : pi.c
Author      : Your names goes here
SCIPER		: Your SCIPER numbers
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include "utility.h"

#define CACHE_LINE_SIZE 64

typedef struct {
    int value;
    int* filler[(CACHE_LINE_SIZE / sizeof(int)) - 1];
}PaddedInt;

int perform_buckets_computation(int, int, int);

int main (int argc, const char *argv[]) {
    int num_threads, num_samples, num_buckets;

    if (argc != 4) {
		printf("Invalid input! Usage: ./sharing <num_threads> <num_samples> <num_buckets> \n");
		return 1;
	} else {
        num_threads = atoi(argv[1]);
        num_samples = atoi(argv[2]);
        num_buckets = atoi(argv[3]);
	}
    
    set_clock();
    perform_buckets_computation(num_threads, num_samples, num_buckets);

    printf("Using %d threads: %d operations completed in %.4gs.\n", num_threads, num_samples, elapsed_time());
    //printf("%.4g\n", elapsed_time());
    return 0;
}

int perform_buckets_computation(int num_threads, int num_samples, int num_buckets) {
    PaddedInt** histograms = (PaddedInt**)malloc(num_threads * sizeof(PaddedInt*));
    for (int i = 0; i < num_threads; i++) {
        histograms[i] = (PaddedInt*)calloc(num_buckets, sizeof(PaddedInt));
        if (histograms[i] == NULL) {
            return -1;
        }
    }
    omp_set_num_threads(num_threads);
    #pragma omp parallel
    {
        int thread = omp_get_thread_num();
        rand_gen generator = init_rand();
        #pragma omp for
        for(int i = 0; i < num_samples; i++){
            int val = next_rand(generator) * num_buckets;
            histograms[thread][val].value++;
        }
        free_rand(generator);
    }

    PaddedInt* histogram = (PaddedInt*)calloc(num_buckets, sizeof(PaddedInt));
    for(int i = 0; i < num_threads; i++) {
        for(int j = 0; j < num_buckets; j++) {
            histogram[j].value += histograms[i][j].value;
        }
        free(histograms[i]);
    }
    free(histograms);
    return 0;
}
