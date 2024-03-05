#include <stdio.h>
#include <stdlib.h>

int get_min_index(int *arr, int start, int end){
	int min_index = start;
	for (int i = start+1; i < end; i++){
		if (arr[i] < arr[min_index]){
			min_index = i;
		}
	}
	return min_index;
}

int main(){
	int N, B, min_index, swaps=0;
	scanf("%d %d",&N, &B);
	
	int *arr = calloc(N, sizeof(int));
	for(int i = 0; i < N; i++){
		scanf("%d", &arr[i]);
	}

	for (int i = 0; i < N; i++){

		if (swaps >= B) break;

		min_index = get_min_index(arr, i, N);
		if (min_index != i){
			int temp = arr[min_index];
			arr[min_index] = arr[i];
			arr[i] = temp;
			swaps++;
		}
	}

	if (swaps < B){
		printf("The array is ");
		for (int i = 0; i < N; i++){
			printf("%d ", arr[i]);
		}
		printf("after %d swaps.", swaps);
		printf("\n");
	}
	else{
		printf("The array is ");
		for (int i = 0; i < N; i++){
			printf("%d ", arr[i]);
		}
		printf("after %d swaps.", B);
		printf("\n");
	}
	free(arr); // 釋放分配的記憶體
	return 0;
}
