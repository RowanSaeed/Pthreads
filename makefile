CC = gcc
CFLAGS = -pthread -Wall

all: matrix

matrix: 7692-matrix.c
	$(CC) $(CFLAGS) 7692-matrix.c -o 7692-matrix

run_input1:
	./7692-matrix input1-matrix.txt

run_input2:
	./7692-matrix input2-matrix.txt

run_input3:
	./7692-matrix input3-matrix.txt

clean:
	rm -f 7692-matrix
