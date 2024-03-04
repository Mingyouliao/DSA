#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 20

int arr[MAX_N];
int N;
int perm[MAX_N], used[MAX_N];   // 用于生成排列的数组和标记数组
int count = 0;
int result[3628800][MAX_N]; // 存储结果，最多为 10! 个排列

// 比较函数，用于 qsort
int cmp(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int isBeautiful2(int perm[], int n) {
    // 因为这个函数现在假设它总是在可以安全检查的情况下被调用，
    // 所以不再需要额外的边界检查。
    int diff1, diff2;
    diff1 = perm[n] - perm[n-1];
    diff2 = perm[n-1] - perm[n-2];
    if (!((diff1 > 0 && diff2 < 0) || (diff1 < 0 && diff2 > 0))) {
        return 0;
    }
    return 1;
}

void backtrack2(int pos) {
    // 现在我们在添加到结果之前检查是否至少有3个元素，且排列是美丽的
    if (pos >= 3 && !isBeautiful2(perm, pos - 1)) return;

    if (pos == N) { // 如果填满了排列，假设在这之前的所有步骤都是美丽的
        for (int i = 0; i < N; i++) {
            result[count][i] = perm[i];
        }
        count++;
        return;
    }

    for (int i = 0; i < N; i++) {
        if (!used[i]) {
            if (i > 0 && arr[i] == arr[i - 1] && !used[i - 1]) continue;
            perm[pos] = arr[i];
            used[i] = 1;
            backtrack2(pos + 1);
            used[i] = 0;
        }
    }
}

int main() {
    // 读取输入
    scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        scanf("%d", &arr[i]);
    }

    // 排序以确保字典序
    qsort(arr, N, sizeof(int), cmp);

    // 生成排列并检查
    backtrack2(0);

    // 输出结果
    printf("%d\n", count); // 输出美丽排列的数量
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    return 0;
}