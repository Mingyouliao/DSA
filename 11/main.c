#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_length 1000000
long long int N, M, Q;
long long int u_i, v_i, l_i;
long long int operation, plan_value, treasure, current_level;
long long int add_to_path5;

long long int binary_search_plan[MAX_length]; // plan 的實作
long long int binary_search_index[MAX_length];
long long int path_5[MAX_length];

long long int binary_search_count; // 用来记录往下走了幾次 // 這也是 max_deque_count
long long int deque_count;

typedef struct StackNode {
    long long int value;
    struct StackNode *next;
} StackNode;

typedef struct Stack {
    StackNode *top;  // 指向栈顶的指针
} Stack;

typedef struct Node {

    long long int dungeon_id;
    long long int parent_child_length;
    long long int stack_max;

    struct Node* parent;
    struct Node* left_sibling;
    struct Node* right_sibling;
    struct Node* child_head;
    struct Node* child_tail;

    Stack stack; // 為了 opertation 4 每個節點都應該要有一個stack
} Node;

// 压栈函数
void push(Stack *stack, long long int value) {
    StackNode *new_stack = malloc(sizeof(StackNode));
    new_stack->value = value;
    new_stack->next = stack->top;
    stack->top = new_stack;
}
// 从右到左进行DFS
// DFS函数
long long int DFSfromright(Node* node) {
    if (node == NULL) {
        return 0;
    }

    long long int temp = 0;
    Node* cursor = node->child_tail;
    while (cursor != NULL) {
        temp = DFSfromright(cursor);
        if (node->stack.top == NULL || temp >= node->stack.top->value) {
            push(&node->stack, temp);
        }
        cursor = cursor->left_sibling;
    }

    // 在这里调用打印栈的函数
    //printf("Node %lld's stack:", node->dungeon_id);
    //printStack(&node->stack);
    
    return node-> parent_child_length + (node->stack.top ? node->stack.top->value : 0);
}

void pop(Stack *stack) {
    if (stack->top != NULL) {
        StackNode *temp = stack->top;
        stack->top = stack->top->next;
        free(temp);
    }
}

// 打印栈的函数
void printStack(Stack *stack) {
    StackNode *current = stack->top;
    printf("Stack:");
    while (current != NULL) {
        printf(" %lld", current->value);
        current = current->next;
    }
    printf("\n");
}

void printtree(Node* node) {
    if (node == NULL)
        return;

    Node* cursor = node->child_tail;
    while (cursor != NULL) {
        printtree(cursor);
        cursor = cursor->left_sibling;
    }

    // 在这里调用打印栈的函数
    printf("Node %lld's stack:", node->dungeon_id);
    printStack(&node->stack);
}


// 第五題 deque 的建立和操作

typedef struct DequeNode {
    long long int value;
    long long int begin_negative;
    long long int remain_from_current_to_0;
    long long int find_treasure_index;
    long long int treasure_level;

    struct DequeNode *next;
    struct DequeNode *prev;
} DequeNode;

typedef struct Deque {
    DequeNode *head;
    DequeNode *tail;
} Deque;

void initDeque(Deque *deque) {
    deque->head = deque->tail = NULL;
}

int isEmpty(Deque *deque) {
    return deque->head == NULL;
}

// 查看队首元素但不移除
DequeNode* peekFront(Deque *deque) {
    if (isEmpty(deque)) {
        // printf("Deque is empty\n");
        return NULL;
    }
    return deque->head;
}

void pushFront(Deque *deque, long long int value, long long int begin_negative, long long int remain_from_current_to_0, long long int find_treasure_index, long long int treasure_level) {
    DequeNode *node = (DequeNode*)malloc(sizeof(DequeNode));
    node->value = value;
    node->begin_negative = begin_negative;
    node->remain_from_current_to_0 = remain_from_current_to_0;
    node->find_treasure_index = find_treasure_index;
    node->treasure_level = treasure_level;
    node->next = deque->head;
    node->prev = NULL;
    if (isEmpty(deque)) {
        deque->tail = node;
    } else {
        deque->head->prev = node;
    }
    deque->head = node;
}

void popFront(Deque *deque) {
    if (isEmpty(deque)) {
        // printf("Deque is empty\n");
        return;
    }
    DequeNode *temp = deque->head;
    long long int value = temp->value;
    // printf("現在的deque的數量 %lld\n", deque_count);
    // printf("現在的max_deque %lld\n", binary_search_count);
    // printf("value %lld and weight %lld and  find_treasure_index %lld\n", temp->value, temp->begin_negative, temp->find_treasure_index);
    deque->head = deque->head->next;
    if (deque->head == NULL) {
        deque->tail = NULL;
    } else {
        deque->head->prev = NULL;
    }
    free(temp);
    return;
}

void popBack(Deque *deque) {
    if (isEmpty(deque)) {
        // printf("Deque is empty\n");
        return;
    }
    DequeNode *temp = deque->tail;
    long long int value = temp->value;
    // printf("value %lld and begin_negative %lld and remain_0_to_current %lld and find_treasure_index %lld \n", temp->value, temp->begin_negative, temp->remain_from_current_to_0, temp->find_treasure_index);
    if (temp->remain_from_current_to_0 >= 0){
        printf("value remaining is %lld\n", temp->remain_from_current_to_0);
    }
    else{
        printf("value lost at %lld\n", temp->begin_negative);
    }
    deque->tail = deque->tail->prev;
    if (deque->tail == NULL) {
        deque->head = NULL;
    } else {
        deque->tail->next = NULL;
    }
    free(temp);
    return;
}

long long int plan_3(long long int treasure_value){

    if (treasure_value - binary_search_plan[binary_search_count] >= 0){
        return (treasure_value - binary_search_plan[binary_search_count]);
    }
    else{
        long long int l = 0;
        long long int r = binary_search_count + 1;
        long long int mid;
        long long int result;

        while (l < r){
            mid = floor( (l+r) / 2 );
            if (treasure_value - (binary_search_plan[binary_search_count] - binary_search_plan[mid]) >= 0){
                r = mid;
                result = mid;
            }
            else{
                l = mid + 1;
            }
        }
        // printf("這是他往上推最多可以推到哪邊不會變成0 %lld\n", binary_search_index[result]);
        return binary_search_index[result - 1];
    }
}

void printDeque(Deque *deque) {
    DequeNode *current = deque->head;
    printf("Current Deque:\n");
    while (current != NULL) {
        printf("value %lld and begin_negative %lld and remain_0_to_current %lld treasure_level %lld \n", current->value, current->begin_negative, current->remain_from_current_to_0, current->treasure_level);
        current = current->next;
    }
}


long long int main() {

    scanf("%lld %lld %lld", &N, &M, &Q);

    Node** dungeons = malloc(N*sizeof(Node*));
    
    for (long long int i = 0; i < N; i++){
        Node* dungeon = (Node*)malloc(sizeof(Node));

        dungeon->dungeon_id = 0;
        dungeon->parent_child_length = 0;
        
        dungeon->parent = NULL;
        dungeon->left_sibling = NULL;
        dungeon->right_sibling = NULL;
        dungeon->child_head = NULL;
        dungeon->child_tail = NULL;
        dungeon->stack.top = NULL;

        dungeons[i] = dungeon;
    }

    //建樹
    for (long long int i = 0; i < M; i++){
        scanf("%lld %lld %lld", &u_i, &v_i, &l_i);

        dungeons[u_i]->dungeon_id = u_i;
        dungeons[v_i]->dungeon_id = v_i;

        dungeons[v_i]->parent_child_length = l_i; // 要去做 operation  3 4 5 的 stack 時要用的

        dungeons[v_i]->parent = dungeons[u_i];

        if (dungeons[u_i]->child_tail == NULL){
            dungeons[u_i]->child_head = dungeons[v_i];
            dungeons[u_i]->child_tail = dungeons[v_i];
        }
        else{
            dungeons[u_i]->child_tail->right_sibling = dungeons[v_i];
            dungeons[v_i]->left_sibling = dungeons[u_i]->child_tail;
            dungeons[u_i]->child_tail = dungeons[v_i];
        }
    }
    //DFS tree to construct stack for each node
    DFSfromright(dungeons[0]);

    Node* rightnow_point = dungeons[0];
    binary_search_count = 0;
    current_level = 0;
    add_to_path5 = 0;
    binary_search_plan[binary_search_count] = rightnow_point->parent_child_length;
    binary_search_index[binary_search_count] = rightnow_point->dungeon_id;
    path_5[binary_search_count] = 0;

    //deque to construct 5; use the same count as plan;
    Deque deque;
    initDeque(&deque);


    for (long long int i = 0; i < Q; i++){
        scanf("%lld", &operation);

        if (operation == 1){ // downstream
            if (rightnow_point->child_head){

                current_level += 1;
                rightnow_point = rightnow_point->child_head;    
                rightnow_point->parent->child_head = rightnow_point->right_sibling;
                
                binary_search_count += 1; // 從 dungeon 0 開始記錄往下走了幾次 // 這個也是 deque最大的多少

                path_5[binary_search_count] = path_5[binary_search_count - 1];

                binary_search_plan[binary_search_count] = binary_search_plan[binary_search_count-1] + rightnow_point->parent_child_length; // 從 dungeon 0 開始記錄往下走的累計長度
                binary_search_index[binary_search_count] = rightnow_point->dungeon_id;

                if (rightnow_point->stack.top){
                    rightnow_point->stack_max = rightnow_point->stack.top->value;
                }

                printf("%lld\n", rightnow_point->dungeon_id);
            }
            else{
                printf("-1\n");
            }
        }

        // 現在的問題出現在，我把stack 的 最大值刪掉後，但我直接在2 ， parent stack 的就刪不掉最大值了。
        else if (operation == 2){ // upstream
            if (rightnow_point->parent){

                long long int expected_value = rightnow_point->parent_child_length +
                                   (rightnow_point->stack_max ? rightnow_point->stack_max : 0);

                // 如果父节点的栈不为空，并且栈顶值等于预期值，则移除栈顶
                if (rightnow_point->parent->stack.top && expected_value == rightnow_point->parent->stack.top->value) {
                    pop(&rightnow_point->parent->stack);
                }

                //printtree(dungeons[0]);

                // printf("\n現在實行pop front喔\n");
                DequeNode *front = peekFront(&deque);
                if (path_5[binary_search_count] > path_5[binary_search_count - 1]){
                    popFront(&deque);
                    deque_count -= 1;
                    path_5[binary_search_count - 1] = path_5[binary_search_count] - 1;
                }


                binary_search_plan[binary_search_count] = 0;
                binary_search_index[binary_search_count] = 0;

                binary_search_count -= 1; 

                rightnow_point = rightnow_point->parent;

                // printf("\npop完成\n");
                current_level -= 1;

                printf("%lld\n", rightnow_point->dungeon_id);
            }
            else{
                printf("-1\n");
            }
        }

        else if (operation == 3){
            scanf("%lld", &plan_value);
            // printf("\n");
            // for (long long int i = 0; i <= binary_search_count; i++){
            //     printf("%lld and index %lld ->", binary_search_plan[i], binary_search_index[i]);
            // }
            // printf("\n");
            // printf("binary_search_count = %lld, binary_search_plan = %lld\n", binary_search_count,binary_search_plan[binary_search_count]);
            if (plan_value - binary_search_plan[binary_search_count] >= 0){
                printf("0\n");
            }
            else{
                long long int l = 0;
                long long int r = binary_search_count + 1;
                long long int mid;
                long long int result;

                while (l < r){
                    mid = floor( (l+r) / 2 );
                    if (plan_value - (binary_search_plan[binary_search_count] - binary_search_plan[mid]) >= 0){
                        r = mid;
                        result = mid;
                    }
                    else{
                        l = mid + 1;
                    }
                }
                printf("%lld\n", binary_search_index[result]);
            }
        }

        else if (operation == 4){
            // printf("%lld\n", rightnow_point->dungeon_id);

            // if (rightnow_point->child_head == NULL){
            //     printf("NULL\n");
            // }
            if (rightnow_point->child_head != NULL){
                printf("%lld\n", rightnow_point->stack.top->value);
            }

            else{
                printf("0\n");
            }
        }

        else if (operation == 5){

            scanf("%lld", &treasure);

            if (binary_search_count != 0)
                path_5[binary_search_count] += 1;

            long long int remain_value = treasure - binary_search_plan[binary_search_count];
            long long int temp_negative = plan_3(treasure);

            pushFront(&deque, treasure, temp_negative, remain_value, rightnow_point->dungeon_id, current_level);

            
            deque_count += 1;
            

            if (deque_count > binary_search_count){
                // printf("\n現在實行pop喔\n");
                popBack(&deque);
                // printf("\npop完成\n");
                deque_count -= 1;
            }
            // printf("現在的deque的數量 %lld\n", deque_count);
            // printf("現在的max_deque %lld\n", binary_search_count);
            // // 打印Deque的内容
            // printf("\n");
            // printDeque(&deque);
            // printf("\n");
        }
        // printf("現在的deque的數量 %lld\n", deque_count);
        // printf("現在的max_deque %lld\n", binary_search_count);
        // printf("現在的current_level %lld\n", current_level);
        // // printf("\n");
        // // printDeque(&deque);
        // // printf("\n");
        // printf("現在這個是path_5\n");
        // for (long long int i = 0; i <= binary_search_count; i++){
        //     printf("%lld ->", path_5[i]);
        // }
        // printf("\n");
        
    }
    return 0;
    
}
