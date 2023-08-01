CC = gcc
CFLAGS = -O3 -fopenmp
SRC = mergesort.c
EXECUTABLE = mergesort

all:
	$(CC) $(CFLAGS) $(SRC) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)