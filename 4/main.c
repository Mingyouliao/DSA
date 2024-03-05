#include <stdio.h>
#include <stdlib.h>

int insert(int *arr, int start){
    int data = arr[start];
    int i = start - 1;
    int move = 0;
    while (i > 0 && arr[i] > data){
        arr[i+1] = arr[i];
        i--;
        move++;
    }
    arr[i+1] = data;
    return move;
}

int main(){
	int N, B, backmoves = 0;
	scanf("%d %d",&N, &B);
	
	int *arr = calloc(N, sizeof(int));
	for(int i = 0; i < N; i++){
		scanf("%d", &arr[i]);
	}

	for (int i = 1; i < N; i++){

		if (backmoves >= B) break;
        backmoves += insert(arr, i);
	}

	if (backmoves < B){
		printf("The array is ");
		for (int i = 0; i < N; i++){
			printf("%d ", arr[i]);
		}
		printf("after %d back-moves.", backmoves);
		printf("\n");
	}
	else{
		printf("The array is ");
		for (int i = 0; i < N; i++){
			printf("%d ", arr[i]);
		}
		printf("after %d back-moves.", B);
		printf("\n");
	}

    free(arr); // 釋放分配的記憶體
	return 0;
}
