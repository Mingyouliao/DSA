#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Stack {
    struct Stack* below;
    long long int data;
    char token;
} Stack;

void push(Stack** stack, long long int data, char token) {
    Stack* new_node = (Stack*)malloc(sizeof(Stack));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->token = token;
    new_node->below = *stack;
    *stack = new_node;
}

Stack* pop(Stack** stack) {
    if (!(*stack)) {
        return NULL;
    }
    Stack* top = *stack;
    *stack = (*stack)->below;
    return top;
}

void free_node(Stack* node) {
    free(node);
}

int is_empty(Stack* stack) {
    return !stack;
}

char peek(Stack* stack) {
    return stack ? stack->token : '\0';
}

int precedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
        case '%':
            return 2;
        default:
            return 0;
    }
}

int has_higher_precedence(char op1, char op2) {
    int p1 = precedence(op1);
    int p2 = precedence(op2);
    // 如果 op1 的優先級高於 op2，或者兩者優先級相同（並且是左結合），則返回 1。
    return p1 >= p2;
}



long long int calculate(long long int a, char op, long long int b) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b ? a / b : 0; // Add check for division by zero
        case '%': return b ? a % b : 0;
        default: return 0;
    }
}

void infix_to_postfix(const char* infix, char* postfix) {
    Stack* stack = NULL;
    int j = 0;
    for (int i = 0; infix[i]; i++) {
        if (isdigit(infix[i])) {
            // Support negative numbers
            if (infix[i] == '-' && isdigit(infix[i + 1])) {
                postfix[j++] = infix[i++];
            }
            while (isdigit(infix[i])) {
                postfix[j++] = infix[i++];
            }
            postfix[j++] = ' ';
            i--;
        } else if (infix[i] == '(') {
            push(&stack, 0, '(');
        } else if (infix[i] == ')') {
            while (!is_empty(stack) && peek(stack) != '(') {
                postfix[j++] = pop(&stack)->token;
                postfix[j++] = ' ';
            }
            free_node(pop(&stack)); // Free the popped '('
        } else {
            while (!is_empty(stack) && peek(stack) != '(' && has_higher_precedence(peek(stack), infix[i])) {
                postfix[j++] = pop(&stack)->token;
                postfix[j++] = ' ';
            }
            push(&stack, 0, infix[i]);
        }
    }
    while (!is_empty(stack)) {
        postfix[j++] = pop(&stack)->token;
        postfix[j++] = ' ';
    }
    if (j > 0) { // Remove trailing space and add null terminator
        postfix[j - 1] = '\0';
    } else {
        postfix[j] = '\0';
    }
}

long long int postfix_eval(char* postfix) {
    Stack* stack = NULL;
    char* token = strtok(postfix, " ");
    while (token) {
        if (isdigit(token[0])) {
            push(&stack, atoll(token), 0);
        } else {
            Stack* n2_node = pop(&stack);
            Stack* n1_node = pop(&stack);
            long long int result = calculate(n1_node->data, token[0], n2_node->data);
            free_node(n2_node);
            free_node(n1_node);
            push(&stack, result, 0);
        }
        token = strtok(NULL, " ");
    }
    Stack* result_node = pop(&stack);
    long long int result = result_node->data;
    free_node(result_node); // Free the final node
    return result;
}


int main() {
    for (int i = 0; i < 3; ++i) {
        char infix[4001];
        char postfix[8002] = {0};

        scanf("%3999s", infix);
        infix_to_postfix(infix, postfix);

        char postfix_copy[8002] = {0};
        strcpy(postfix_copy, postfix);
        long long result = postfix_eval(postfix_copy); // 現在僅打印結果
        for (int j = 0; postfix[j]; ++j) {
            if (postfix[j] != ' ') { // 跳过空格
                printf("%c", postfix[j]);
            }
        }
        printf("=%lld\n", result); // 打印计算结果
    }
    return 0;
}

