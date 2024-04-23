#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    long long data;
    struct Node *next;
} Node;

Node* createNode(long long data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        perror("Error creating a new node");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void printList(Node *node) {
    while (node != NULL) {
        printf("%lld -> ", node->data);
        node = node->next;
    }
    printf("NULL\n");
}

int hasCycle(Node *head) {
    Node *hare, *tortoise;

    hare = tortoise = head;

    if (hare != NULL) {
        printf("%lld ", hare->data);
    }

    while (hare != NULL && hare->next != NULL) {

        hare = hare->next->next;
        tortoise = tortoise->next;

        if (hare != NULL) {
        printf("%lld ", hare->data);
        }

        if (hare == tortoise) {
            return 1;
        }
    }
    return 0;
}

int main() {
    long long N;
    scanf("%lld", &N);

    Node *nodes[N+1]; // Using 1-based indexing for convenience

    for (long long i = 1; i <= N; ++i) {
        nodes[i] = createNode(0); // Initialize with dummy data
    }

    for (long long i = 1; i <= N; ++i) {
        long long data, nextIndex;
        scanf("%lld %lld", &data, &nextIndex);
        nodes[i]->data = data;
        if (nextIndex > 0 && nextIndex <= N) {
            nodes[i]->next = nodes[nextIndex];
        }
    }

    Node *head = nodes[1]; // Assuming head is the first node
    hasCycle(head);
    printf("\n");

    // if (hasCycle(head)) {
    //     printf("Cycle detected.\n");
    // } else {
    //     printf("No cycle detected.\n");
    // }
    
    return 0;
}
