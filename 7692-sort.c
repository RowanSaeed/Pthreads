#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int *Array;
    int low;
    int high;
} ThreadData;

void merge(int Array[], int low, int mid, int high) {
    int nL = mid - low + 1;
    int nR = high - mid;

    int L[nL];
    int R[nR];

    for (int i = 0; i < nL; i++) {
        L[i] = Array[low + i];
    }

    for (int i = 0; i < nR; i++) {
        R[i] = Array[mid + 1 + i];
    }

    int i = 0;
    int j = 0;
    int k = low;

    while (i < nL && j < nR) {
        if (L[i] < R[j]) {
            Array[k] = L[i];
            k++;
            i++;
        } else {
            Array[k] = R[j];
            k++;
            j++;
        }
    }

    while (i < nL) {
        Array[k] = L[i];
        k++;
        i++;
    }

    while (j < nR) {
        Array[k] = R[j];
        k++;
        j++;
    }
}
void *mergesort_threaded(void *arg) {
    ThreadData *currentdata = (ThreadData *)arg;
    int low = currentdata->low;
    int high = currentdata->high;

    if (low < high) {
        int mid = (low + high) / 2;

        // initialize left, right thread
        pthread_t left_thread, right_thread;
   	
   	// data for left and right thread
        ThreadData left_data = { currentdata->Array, low, mid };
        ThreadData right_data = { currentdata->Array, mid + 1, high };
	
	// create left and right thread
        pthread_create(&left_thread, NULL, mergesort_threaded, &left_data);
        pthread_create(&right_thread, NULL, mergesort_threaded, &right_data);

        // Wait all threads finish
        pthread_join(left_thread, NULL);
        pthread_join(right_thread, NULL);

        // Merge the two halves
        merge(currentdata->Array, low, mid, high);
    }


pthread_exit(NULL);
   
}

void mergesort(int Array[], int size) {
    ThreadData data = { Array, 0, size - 1 };  // arr, low, high
    pthread_t initial_thread;
    pthread_create(&initial_thread, NULL, mergesort_threaded, &data);
    pthread_join(initial_thread, NULL);
}

int main(int argc, char *argv[]) {
    // if no file  name entered
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

   // read number of elements from file
    int n;
    fscanf(file, "%d", &n);
    

    int *A = malloc(n * sizeof(int));
    if (A == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        fscanf(file, "%d", &A[i]);
    }

    fclose(file);

    // merge sort
    mergesort(A, n);
   

    // Print sorted array
    printf("Sorted array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");

    free(A);

    return 0;
}
