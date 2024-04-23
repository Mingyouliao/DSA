#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void Swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

int Parent(int i) {
    return (i - 1) / 2;
}

void HeapIncreaseKey(int *A, int i, int key) {
    if (key < A[i]) {
        return;
    }
    A[i] = key;
    while (i > 0 && A[Parent(i)] < A[i]) {
        Swap(&A[i], &A[Parent(i)]);
        i = Parent(i);
    }
}

void MaxHeapInsert(int *A, int *heapSize, int key) {
    *heapSize = *heapSize + 1;
    A[*heapSize - 1] = INT_MIN;
    HeapIncreaseKey(A, *heapSize - 1, key);
}

int main() {
    int N;
    scanf("%d", &N); // Read the number of elements

    // Dynamically allocate the array for heap
    int *A = (int *)malloc((N + 1) * sizeof(int)); // Extra space for ease of implementation (1-indexed)

    int heapSize = 0;
    int key;

    // Read the elements and insert them into the heap
    for (int i = 0; i < N; i++) {
        scanf("%d", &key);
        MaxHeapInsert(A, &heapSize, key);
    }

    // Output the max-heap
    for (int i = 0; i < heapSize; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");

    free(A); // Free the dynamically allocated memory
    return 0;
}
