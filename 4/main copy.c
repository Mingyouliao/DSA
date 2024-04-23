#include <stdio.h>
#include <stdlib.h>

// 假如 插入的值全部都只能在最底層 長不上去，所以fastget 就等於 slowget，會因為這樣TLE嘛？
// 7
// 3 2
// 3 4
// 3 6
// 3 8
// 3 10
// 3 12
// 3 14
// SkipList Structure:
// Level 0: 14 12 10 8 6 4 2 
// 層數也怪怪的 check 一下

#define MAX_LEVEL 64

typedef struct Node {
    long long key;
    struct Node *next;
    struct Node *below;
} Node;

typedef struct SkipList {
    Node *head[MAX_LEVEL];
    int maxLevel;
} SkipList;


Node* createNode(long long key) {
    Node *n = (Node*)malloc(sizeof(Node));
    n->key = key;
    n->next = NULL;
    n->below = NULL;
    return n;
}

SkipList* createSkipList() {
    SkipList *sl = (SkipList*)malloc(sizeof(SkipList));
    for (int i = 0; i < MAX_LEVEL; i++) {
        sl->head[i] = createNode(-1);
    }
    for (int i = MAX_LEVEL - 1; i > 0; i--) {
        sl->head[i]->below = sl->head[i - 1];
    }
    sl->maxLevel = 0;
    return sl;
}

int determine(long long k, int i) {
    if ((k >> (i-1)) % 2 == 1) {
        return 1;
    } else {
        return 0;
    }
}

Node* fastGet_find(Node* start, long long key) {
    Node* node = start;
    while (node->next != NULL && node->next->key >= key) {
        node = node->next;
    }
    return node;
}

// 有機會改成儲存位置後，一次插入
void insert(SkipList *sl, long long key) {
    Node *current = NULL, *newNode = NULL, *temp = NULL;
    int levelChosen = 0;

    // Determine which levels the new node should appear on
    for (int i = 0; i < MAX_LEVEL; i++) {
        if (determine(key, i + 1) == 0) {
            levelChosen = i;
            break; // Found the highest level, exit loop
        }
    }
    int layer_down = levelChosen;
    current = sl->head[levelChosen];

    while (layer_down >= 0){
        while (current->next != NULL && current->next->key >= key) {
            current = current->next;
        }
        newNode = createNode(key); // Create a new node
        // Insert the new node
        newNode->next = current->next;
        current->next = newNode;
        if (temp != NULL) {
            temp->below = newNode;
        }
        temp = newNode;
        if(layer_down > 0) {
            current = current->below;
        }
        layer_down--;
    }

    if (levelChosen > sl->maxLevel){
        sl->maxLevel = levelChosen;
    }
    
	// // Traverse the SkipList from the top level to the bottom level
    // for (int i = levelChosen; i >= 0; i--) {
    //     current = fastGet_find(current, key); // Find the appropriate insertion point
    //     newNode = createNode(key); // Create a new node
		
    //     // Insert the new node
    //     newNode->next = current->next;
    //     current->next = newNode;
		
	// 	if (temp != NULL) {
	// 		temp->below = newNode;
	// 	}
	// 	temp = newNode;
	// 	// Move to the next level
	// 	if (i > 0) {
	// 		current = current->below;
	// 	}
    // }
    // // Update the maxLevel of the SkipList if necessary
    // if (levelChosen > sl->maxLevel){
    //     sl->maxLevel = levelChosen;
    // }
    //printf("sl->maxLevel = %d\n", sl->maxLevel);
}

void slowget(SkipList *sl, long long key) {
    int temp = 0;
    Node *current = sl->head[0];  // 开始于最底层的头节点
	while(current->next != NULL && current->next->key >= key) {
		current = current->next;
        printf("%lld ", current->key);
        temp++;
	}

    if(temp == 0) {
        printf("-1\n");
    } else {
        printf("\n");
    }
}

void fastget(SkipList *sl, long long key) {

    if (sl->head[0]->next == NULL){
        printf("-1\n");
        return;
    }

    Node *current = sl->head[sl->maxLevel]; // Start at the topmost level that has nodes
    int current_layer = sl->maxLevel;
    int value = 0;
    int find = 0;
    // Traverse down the levels
    // 倒數最後第二層
    while (current->below != NULL) {
        // Traverse right on the same level
        // 這邊要改，找到值後 就直接往下印出來不要在判斷。
        while (current->next != NULL && current->next->key >= key) {
            current = current->next;
            //printf("有跑到我喔\n");
            //避免current 一直在header的位置，不要印出header
            printf("%lld ", current->key); // Print the key of the traversed node
            value = 1;
        }

        if (current->key == key)
        {
            break;
        }

        // Move down a level
        current = current->below;
        current_layer--;
        if (current->key != -1){
            printf("%lld ", current->key); // Print the key of the traversed node
            value = 1;
        }
    }

    //最後一層
    if (current->key == key)
    {
        for (int i = current_layer - 1; i >= 0; i--) {
            printf("%lld ", current->key); // Print the key of the traversed node
        }
    }

    else{
        while (current->next != NULL && current->next->key >= key) {
            current = current->next;
            printf("%lld ", current->key); // Print the key of the traversed node
            value = 1;
        }
    }

    if (!value) {
        printf("-1");
    }

    printf("\n");
}

void number_remove(SkipList *sl, long long key) {
    Node *current = sl->head[sl->maxLevel]; // Start at the topmost layer
    Node *temp = NULL;

    // Flag to check if we have removed any node
    int removed = 0;

    // We will go down each level and remove the node with the given key
    while (current->below != NULL) {

        while (current->next != NULL && current->next->key >= key) {
            current = current->next; // Move right as long as the next key is less than the search key
        }

        // If the next node is the target node, remove it
        if (current->next  && current->next->key == key) {
            temp = current->next; // Hold the node to be removed
            current->next = temp->next; // Bypass the node to be removed
            free(temp); // Free the removed node
            removed = 1; // Set the flag since we removed a node
        }

        // Move down a level
        current = current->below;
    }

    // If we removed a node, check if we need to update the SkipList's max level
    if (removed) {
        while (sl->maxLevel > 0 && sl->head[sl->maxLevel]->next == NULL) {
            sl->maxLevel--;
        }
    }
}


void printSkipList(SkipList *sl) {
    printf("SkipList Structure:\n");
    for (int i = sl->maxLevel; i >= 0; i--) {
        Node *node = sl->head[i]->next;
        printf("Level %d: ", i);
        while (node) {
            printf("%lld ", node->key);
            node = node->next;
        }
        printf("\n");
    }
}

int main() {
    SkipList *sl = createSkipList();

	int times;
	scanf("%d", &times);

	for (int i = 0; i < times; i++) {
		int opeartion;
        long long key;
		scanf("%d %lld", &opeartion, &key);
		switch (opeartion)
		{
		case 1:
			slowget(sl, key);
			break;
        case 2:
        	fastget(sl, key);
        	break;
		case 3:
			insert(sl, key);
			break;
        case 4:
            number_remove(sl, key);
            printSkipList(sl);
            break;
		}
	}
    //printSkipList(sl);
    return 0;
}
