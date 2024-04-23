/* C program to implement Binomial Heap tree */
// 現在的問題是 union 的地方有問題 
 
#include <stdio.h>
#include <stdlib.h>

#define MAX_PRINTERS 1000000
#define MAX_OPERATION 1000000

struct node {
    long long n;
    long long degree;
    long long job_id;
    struct node* parent;
    struct node* child;
    struct node* sibling;
};

struct node* printer[MAX_PRINTERS];
long long number[MAX_OPERATION];
 
struct node* MAKE_bin_HEAP();
long long bin_LINK(struct node*, struct node*);
struct node* CREATE_NODE(long long, long long);
struct node* bin_HEAP_UNION(struct node*, struct node*);
struct node* bin_HEAP_INSERT(struct node*, struct node*, long long);
struct node* bin_HEAP_MERGE(struct node*, struct node*);
struct node* bin_HEAP_EXTRACT_MAX(struct node*, long long);
struct node* REVERT_LIST(struct node*);
void display(struct node* H, long long level);
 
struct node* MAKE_bin_HEAP() {
    struct node* np;
    np = NULL;
    return np;
}
 
struct node * H = NULL;
struct node *Hr = NULL;

struct node* CREATE_NODE(long long priority, long long job_id) {
    struct node* p;//new node;
    p = (struct node*) malloc(sizeof(struct node));
    p->n = priority;
    p->job_id = job_id;
    return p;
}
 
struct node* bin_HEAP_INSERT(struct node* H, struct node* x, long long printer_id) {
    struct node* H1 = MAKE_bin_HEAP();
    x->parent = NULL;
    x->child = NULL;
    x->sibling = NULL;
    x->degree = 0;
    H1 = x;
    H = bin_HEAP_UNION(H, H1);
    return H;
}

struct node* bin_HEAP_UNION(struct node* H1, struct node* H2) {
    struct node* prev_x;
    struct node* next_x;
    struct node* x;
    struct node* H = MAKE_bin_HEAP();
    H = bin_HEAP_MERGE(H1, H2);
    // printf("\n看merger完長怎樣\n");
    // display(H,0);
    // printf("\nmerger完成\n"); 
    if (H == NULL)
        return H;
    prev_x = NULL;
    x = H;
    next_x = x->sibling;
    while (next_x != NULL) {
        if ((x->degree != next_x->degree) || ((next_x->sibling != NULL) && (next_x->sibling)->degree == x->degree)) {
            prev_x = x;
            x = next_x;
        } else {
            if (x->n >= next_x->n) { // 這邊用 >= 就是 max heap
                // printf("\ncheck 要union的值\n");
                // printf("x = %lld next = %lld", x->degree, next_x->degree);
                // printf("\ncheck完成 要union的值\n");
                x->sibling = next_x->sibling;
                // printf("\ncheck 要union的值\n");
                // printf("x = %lld next = %lld", x->n, next_x->n);
                // printf("\ncheck完成 要union的值\n");
                bin_LINK(next_x, x);
            } else {
                if (prev_x == NULL)
                    H = next_x;
                else
                    prev_x->sibling = next_x;
                // printf("\ncheck 要union的值\n");
                // printf("x = %lld next = %lld", x->n, next_x->n);
                // printf("\ncheck完成 要union的值\n");
                bin_LINK(x, next_x);
                x = next_x;
            }
        }
        next_x = x->sibling;
    }
    return H;
}

// 這是把 y 變到 z 的 下面 
long long bin_LINK(struct node* y, struct node* z) {
    y->parent = z;
    y->sibling = z->child;
    z->child = y;
    z->degree = z->degree + 1;
}
 
struct node* bin_HEAP_MERGE(struct node* H1, struct node* H2) {
    if (!H1) return H2;
    if (!H2) return H1;

    struct node* head = NULL;  // 新链表的头部
    struct node* tail = NULL;  // 用于跟踪新链表的尾部

    // 合并两个链表，保证degree的顺序
    while (H1 && H2) {
        struct node* temp;
        if (H1->degree <= H2->degree) {
            temp = H1;
            H1 = H1->sibling;
        } else {
            temp = H2;
            H2 = H2->sibling;
        }

        if (!tail) {
            head = tail = temp;  // 初始化头部和尾部
        } else {
            tail->sibling = temp;  // 连接到链表尾部
            tail = temp;  // 更新尾部指针
        }
    }

    // 连接剩余的部分
    if (H1) {
        tail->sibling = H1;
    } else if (H2) {
        tail->sibling = H2;
    }

    return head;
}

struct node* bin_HEAP_EXTRACT_MAX(struct node* H1, long long printer_id) { // 直接讀printer的 Heap
    long long max;
    struct node* t = NULL;
    struct node* new_head;
    struct node* x = H1;
    struct node* p;

    if (x == NULL) {
        return x;
    }
    p = x;
    max = x->n;
    while (p->sibling != NULL) {
        if ((p->sibling)->n > max) {
            max = (p->sibling)->n;
            t = p;
            x = p->sibling;
        }
        p = p->sibling;
    }

    if (t == NULL && x->sibling == NULL){ // binomial tree 裡面只有他一個
        H1 = NULL;
    }
    else if (t == NULL){ // binomial tree 裡面還有其他人，但最小值就是第一個點
        H1 = x->sibling;
    }
    else{
        t->sibling = x->sibling; // 把抽離最小值的地方前後補起來
    }

    if (x->child != NULL) {
        new_head = REVERT_LIST(x->child);
    }
    else
        new_head = NULL;

    printer[printer_id] = bin_HEAP_UNION(H1, new_head);

    return x;
}
 
struct node* REVERT_LIST(struct node* y) {
    struct node* child = y;
    struct node* new_roots = NULL;
    while (child != NULL) {
        struct node* next_child = child->sibling;
        child->sibling = new_roots;
        child->parent = NULL;
        new_roots = child;
        child = next_child;
    }
    return new_roots;
}

void display(struct node* H, long long level) {
    struct node* p = H;
    while (p != NULL) {
        for (long long i = 0; i < level; i++) {  // 添加缩进以显示层次结构
            printf("  ");
        }
        printf("%lld and degree %lld\n ", p->n, p->degree);  // 打印当前节点的值

        if (p->child != NULL) {
            for (long long i = 0; i < level; i++) {  // 适当增加缩进
                printf("  ");
            }
            printf("  Children of %lld and degree %lld : \n", p->n, p->degree);
            display(p->child, level + 2);  // 递归打印子节点，并增加缩进
        }
        p = p->sibling;  // 移至下一个兄弟节点
    }
}

long long main() {

   long long N, M;
    struct node* p;
    struct node* np;

    scanf("%lld %lld", &N, &M);

    for (int i = 0; i < N; i++) {
        printer[i] = MAKE_bin_HEAP();
    }


    for (long long i = 1; i <= M; i++) {
        long long type, job_id, priority, printer_id, printer_id_move_1, printer_id_move_2;
        scanf("%lld", &type);
        if (type == 1) {
            scanf("%lld %lld %lld", &job_id, &priority, &printer_id);
            np = CREATE_NODE(priority, job_id);
            printer[printer_id - 1] = bin_HEAP_INSERT(printer[printer_id - 1], np, printer_id - 1);
            number[printer_id - 1] += 1;
            // H = bin_HEAP_INSERT(H, np); // 這之後要換成不同的printer_id
            printf("%lld jobs waiting on printer %lld\n", number[printer_id - 1], printer_id); // 這邊的1之後要改掉
            // printf("\nNOW THE HEAP IS:\n");
            // display(printer[printer_id - 1], 0); // 用來debug
        }
        if (type == 2) {
            scanf("%lld", &printer_id);
            p = bin_HEAP_EXTRACT_MAX(printer[printer_id - 1], printer_id - 1); // 這邊是直接去改裡面的東西，所以比較麻煩
            if (p != NULL) {
                // printf("\nTHE EXTRACTED priority IS %d", p->n);
                // printf("\nTHE EXTRACTED job_id IS %d", p->job_id);
                number[printer_id - 1] -= 1;
                printf("%lld printed\n", p->job_id);
            }else{
                printf("no documents in queue\n");
            }
            // printf("\nNOW THE HEAP IS:\n"); // debug
            // display(printer[printer_id - 1], 0);
        }
        if (type == 3) {
            scanf("%lld %lld", &printer_id_move_1, &printer_id_move_2);
            printer[printer_id_move_2 - 1] = bin_HEAP_UNION(printer[printer_id_move_1 - 1], printer[printer_id_move_2 - 1]);
            printer[printer_id_move_1 - 1] = MAKE_bin_HEAP();

            number[printer_id_move_2 - 1] = number[printer_id_move_2 - 1] + number[printer_id_move_1 - 1];
            number[printer_id_move_1 - 1] = 0;
            printf("%lld jobs waiting on printer %lld after moving\n", number[printer_id_move_2 - 1], printer_id_move_2);
            // 這邊要合併 兩個 printer
            // H = bin_HEAP_UNION(H1, Hr);
        }

    }
    return 0;
}
