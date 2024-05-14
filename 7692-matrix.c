#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int row;
    int col;
} ThreadData;

int **A, **B, **C;
int r1, c1, r2, c2;


// function 1
void *multiply_element(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int row = data->row;
    int col = data->col;

    C[row][col] = 0;          // initialize c 0s
    for (int m = 0; m < c1; m++) {
        C[row][col] += A[row][m] * B[m][col];
    }

    pthread_exit(NULL);
}


// function 2
void *multiply_row(void *arg) {
    int row = *((int *)arg);
    for (int j = 0; j < c2; j++) {
        C[row][j] = 0;
        for (int m = 0; m < c1; m++) {
            C[row][j] += A[row][m] * B[m][j];
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    // error input file not entered
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    // read matrices from file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
 
    fscanf(file, "%d %d", &r1, &c1);    // number of rows and cols in A

    // allocate memory for A
    A = (int **)malloc(r1 * sizeof(int *));
    for (int i = 0; i < r1; i++) {
        A[i] = (int *)malloc(c1 * sizeof(int));
    }

    //read matrix A from file
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c1; j++) {
            fscanf(file, "%d", &A[i][j]);
        }
    }

    fscanf(file, "%d %d", &r2, &c2);       // number of rows and cols in B

    // allocate memory for B
    B = (int **)malloc(r2 * sizeof(int *));
    for (int i = 0; i < r2; i++) {
        B[i] = (int *)malloc(c2 * sizeof(int));
    }

    // read matrix B from file
    for (int i = 0; i < r2; i++) {
        for (int j = 0; j < c2; j++) {
            fscanf(file, "%d", &B[i][j]);
        }
    }

    fclose(file);

    // allocate memory for C
    C = (int **)malloc(r1 * sizeof(int *));
    for (int i = 0; i < r1; i++) {
        C[i] = (int *)malloc(c2 * sizeof(int));
    }
    
    // fn to dealloacate memory 
    void deallocateMatrix(int** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
    
   /////////////////////////////////////////////////////////
      
    // allocate memory for threads needed for fn 1
    pthread_t *threads = (pthread_t *)malloc(r1 * c2 * sizeof(pthread_t));
    
    // allocate memory for threads needed for fn 2
    pthread_t *row_threads = (pthread_t *)malloc(r1 * sizeof(pthread_t));
    
    clock_t start, end;

    // check multiplication is possible
    if (c1 == r2) {
    
        
        ThreadData data[r1 * c2];     
        int thread_count = 0;
	
	// fn1
        start = clock();
        for (int i = 0; i < r1; i++) {
            for (int j = 0; j < c2; j++) {
                //setting the row and column for each thread
                data[thread_count].row = i;     
                data[thread_count].col = j;
                // creating threads
                pthread_create(&threads[thread_count], NULL, multiply_element, &data[thread_count]);
                thread_count++;
            }
        }
        
        // waiting for all threads to finish
        for (int i = 0; i < thread_count; i++) {
            pthread_join(threads[i], NULL);
        }
        end = clock();

        printf("\n");
        printf("By Element:");
        printf("\n");
        // print output matrix
        for (int i = 0; i < r1; i++) {
            for (int j = 0; j < c2; j++) {
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }

        // print time elapsed
        printf("Time: %f\n", ((double)(end - start)) / CLOCKS_PER_SEC);
        
	//fn2
        start = clock();
        for (int i = 0; i < r1; i++) {
            int *row_data = (int *)malloc(sizeof(int));
            *row_data = i;
            pthread_create(&row_threads[i], NULL, multiply_row, row_data);
        }

        for (int i = 0; i < r1; i++) {
            pthread_join(row_threads[i], NULL);
        }
        end = clock();

        printf("\n");
        printf("By Row:");
        printf("\n");
        for (int i = 0; i < r1; i++) {
            for (int j = 0; j < c2; j++) {
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }

        printf("Time: %f\n", ((double)(end - start)) / CLOCKS_PER_SEC);
    } else {
        printf("Multiplication invalid\n");
    }


    deallocateMatrix(A, r1);
    deallocateMatrix(B, r2);
    deallocateMatrix(C, r1);
    free(threads);
    free(row_threads);

    return 0;
}
