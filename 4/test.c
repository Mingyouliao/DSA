#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_KEY 1000000000000000000LL // 10^18

void generateTestData(int numOfOperations) {
    srand(time(NULL)); // 初始化随机数种子

    long long *insertedKeys = malloc(numOfOperations * sizeof(long long)); // 存储已插入键值以避免重复
    int insertCount = 0;

    for (int i = 0; i < numOfOperations; i++) {
        //int operation = rand() % 4 + 1; // 随机选择操作类型，1到4之间
        int operation = rand() % 3 + 1; // 随机选择操作类型，1到3之间

        // 随机生成键值，确保不超过约束的最大值
        long long key = llabs(rand() * ((long long)RAND_MAX + 1) + rand()) % (MAX_KEY + 1);

        // 检查重复，如果键值已存在，则重新生成
        int duplicate;
        do {
            duplicate = 0;
            for (int j = 0; j < insertCount; j++) {
                if (insertedKeys[j] == key) {
                    duplicate = 1;
                    key = llabs(rand() * ((long long)RAND_MAX + 1) + rand()) % (MAX_KEY + 1);
                    break;
                }
            }
        } while (duplicate);

        if (operation == 3) {
            insertedKeys[insertCount++] = key; // 存储已插入的键值
        }

        printf("%d %lld\n", operation, key); // 输出操作和键值
    }

    free(insertedKeys);
}

int main() {
    int numOfOperations = 20; // 假设我们要生成 20 次操作，可以调整为最多 10^5
    if (numOfOperations < 1 || numOfOperations > 100000) {
        printf("Number of operations must be between 1 and 100000.\n");
        return 1;
    }

    printf("%d\n", numOfOperations); // 输出操作次数
    generateTestData(numOfOperations);
    return 0;
}
