#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void bin_search(int *arr, int key, int left, int right){
    while (left <= right){
        int mid = floor((left + right) / 2);
        if (arr[mid] == key){
            printf("Searching %d in range [%d, %d].", key, left+1, right+1);
            printf("\n");
            printf("Found at index %d.", mid+1);
            printf("\n");
            return;
        }
        else if (arr[mid] < key){
            printf("Searching %d in range [%d, %d].", key, left+1, right+1);
            printf("\n");
            left = mid + 1;
        }
        else{
            printf("Searching %d in range [%d, %d].", key, left+1, right+1);
            printf("\n");
            right = mid - 1;
        }
    }
    printf("Searching %d in range [%d, %d].", key, left+1, right+1);
    printf("\n");
    printf("Not found.");
    printf("\n");
}

int main(){
    int N, key;
    scanf("%d %d",&N, &key);

    int *arr = calloc(N, sizeof(int));
    for (int i = 0; i < N; i++){
        scanf("%d", &arr[i]);
    }

    bin_search(arr, key, 0, N-1);

    return 0;
}