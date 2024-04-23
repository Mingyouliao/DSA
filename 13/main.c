#include <stdio.h>

void Swap(int *x, int *y, int *swapCount) {
    int temp = *x;
    *x = *y;
    *y = temp;
    (*swapCount)++; // 增加交換次數
}

int Partition(int A[], int l, int r, int *swapCount, int B) {
    int pivot = A[l];
    int i = l + 1;
    int j = r;
    while (i < j) {
        while (i <= r && A[i] <= pivot) i++;
        while (j >= l && A[j] > pivot) j--;
        if (i >= j) break;
        if (*swapCount >= B) return j; // 如果達到交換次數預算則停止
        Swap(&A[i], &A[j], swapCount);
    }
    if (l < j && A[l] > A[j]) {
        if (*swapCount >= B) return j; // 檢查交換次數預算是否已達上限
        Swap(&A[l], &A[j], swapCount);
    }
    return j;
}

int QuickSort(int A[], int l, int r, int *swapCount, int B) {
    if (*swapCount >= B || l >= r) {
        return 0; // 達到交換次數預算或已排序
    }
    int m = Partition(A, l, r, swapCount, B);
    QuickSort(A, l, m - 1, swapCount, B);
    QuickSort(A, m + 1, r, swapCount, B);
}

int main() {
    int N, B;
    scanf("%d %d", &N, &B);
    int A[N];
    int swapCount = 0;

    for (int i = 0; i < N; i++) {
        scanf("%d", &A[i]);
    }

    QuickSort(A, 0, N - 1, &swapCount, B);

    // 輸出結果
    printf("The array is ");
    for (int i = 0; i < N; i++) {
        printf("%d", A[i]);
        if (i < N - 1) {
            printf(" ");
        }
    }
    printf(" after %d swaps.\n", swapCount);

    return 0;
}
