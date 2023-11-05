
CC = gcc
CFLAGS = -std=gnu99 -O3 -fopenmp -Wall

<<<<<<< HEAD
=======

>>>>>>> 337b4a6 (Question1 finished)
all: heatmap sharing

heatmap: heatmap.c utility.h algorithm.c
	$(CC) $(CFLAGS) $< -o $@

sharing: sharing.c utility.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f heatmap sharing
