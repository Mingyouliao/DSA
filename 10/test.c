/* C program to implement Binomial Heap tree */
 
#include <stdio.h>
#include <stdlib.h>
 
struct node {
    int n;
    int degree;
    int job_id;
    struct node* parent;
    struct node* child;
    struct node* sibling;
};
 
struct node* MAKE_bin_HEAP();
int bin_LINK(struct node*, struct node*);
struct node* CREATE_NODE(int, int);
struct node* bin_HEAP_UNION(struct node*, struct node*);
struct node* bin_HEAP_INSERT(struct node*, struct node*);
struct node* bin_HEAP_MERGE(struct node*, struct node*);
struct node* bin_HEAP_EXTRACT_MIN(struct node*);
struct node* REVERT_LIST(struct node*);
void DISPLAY(struct node*);
void display(struct node* H, int level);
void displayBinomialHeap(struct node*);
struct node* FIND_NODE(struct node*, int);
int bin_HEAP_DECREASE_KEY(struct node*, int, int);
int bin_HEAP_DELETE(struct node*, int);
 
int count = 1;
 
struct node* MAKE_bin_HEAP() {
    struct node* np;
    np = NULL;
    return np;
}
 
struct node * H = NULL;
struct node *Hr = NULL;
 
int bin_LINK(struct node* y, struct node* z) {
    y->parent = z;
    y->sibling = z->child;
    z->child = y;
    z->degree = z->degree + 1;
}
 
struct node* CREATE_NODE(int priority, int job_id) {
    struct node* p;//new node;
    p = (struct node*) malloc(sizeof(struct node));
    p->n = priority;
    p->job_id = job_id;
    return p;
}
 
struct node* bin_HEAP_UNION(struct node* H1, struct node* H2) {
    struct node* prev_x;
    struct node* next_x;
    struct node* x;
    struct node* H = MAKE_bin_HEAP();
    H = bin_HEAP_MERGE(H1, H2);
    if (H == NULL)
        return H;
    prev_x = NULL;
    x = H;
    next_x = x->sibling;
    while (next_x != NULL) {
        if ((x->degree != next_x->degree) || ((next_x->sibling != NULL)
                && (next_x->sibling)->degree == x->degree)) {
            prev_x = x;
            x = next_x;
        } else {
            if (x->n >= next_x->n) { // 這邊用 >= 就是 max heap
                x->sibling = next_x->sibling;
                bin_LINK(next_x, x);
            } else {
                if (prev_x == NULL)
                    H = next_x;
                else
                    prev_x->sibling = next_x;
                bin_LINK(x, next_x);
                x = next_x;
            }
        }
        next_x = x->sibling;
    }
    return H;
}
 
struct node* bin_HEAP_INSERT(struct node* H, struct node* x) {
    struct node* H1 = MAKE_bin_HEAP();
    x->parent = NULL;
    x->child = NULL;
    x->sibling = NULL;
    x->degree = 0;
    H1 = x;
    H = bin_HEAP_UNION(H, H1);
    return H;
}
 
struct node* bin_HEAP_MERGE(struct node* H1, struct node* H2) {
    struct node* H = MAKE_bin_HEAP();
    struct node* y;
    struct node* z;
    struct node* a;
    struct node* b;
    y = H1;
    z = H2;
    if (y != NULL) {
        if (z != NULL && y->degree <= z->degree)
            H = y;
        else if (z != NULL && y->degree > z->degree)
            /* need some modifications here;the first and the else conditions can be merged together!!!! */
            H = z;
        else
            H = y;
    } else
        H = z;
    while (y != NULL && z != NULL) {
        if (y->degree < z->degree) {
            y = y->sibling;
        } else if (y->degree == z->degree) {
            a = y->sibling;
            y->sibling = z;
            y = a;
        } else {
            b = z->sibling;
            z->sibling = y;
            z = b;
        }
    }
    return H;
}
 
void DISPLAY(struct node* H) {
    struct node* p;
    if (H == NULL) {
        printf("\nHEAP EMPTY\n");
        return;
    }
    printf("\nTHE ROOT NODES ARE:-\n");
    p = H;
    while (p != NULL) {
        printf("%d", p->n);  // 打印当前节点的值
        if (p->sibling != NULL)
            printf("-->");
        else
            printf("\n");  // 在没有兄弟节点时换行

        if (p->child != NULL) {
            printf("  Children of %d: ", p->n);
            DISPLAY(p->child);  // 递归打印子节点
        }
        p = p->sibling;
    }
}

void display(struct node* H, int level) {
    struct node* p = H;
    while (p != NULL) {
        for (int i = 0; i < level; i++) {  // 添加缩进以显示层次结构
            printf("  ");
        }
        printf("%d\n", p->n);  // 打印当前节点的值

        if (p->child != NULL) {
            for (int i = 0; i < level; i++) {  // 适当增加缩进
                printf("  ");
            }
            printf("  Children of %d: \n", p->n);
            display(p->child, level + 2);  // 递归打印子节点，并增加缩进
        }
        p = p->sibling;  // 移至下一个兄弟节点
    }
}

// 递归地显示每个节点及其子节点
void displayBinomialHeap(struct node* root) {
    while (root != NULL) {
        printf("Root: %d -> ", root->n);
        struct node* child = root->child;
        if (child) {
            printf("Children: ");
            while (child != NULL) {
                printf("%d ", child->n);
                child = child->sibling;
            }
        } else {
            printf("No children");
        }
        printf("\n");
        root = root->sibling;
    }
}
 
struct node* bin_HEAP_EXTRACT_MIN(struct node* H1) {
    int min;
    struct node* t = NULL;
    struct node* new_head;
    struct node* x = H1;
    struct node* p;
    Hr = NULL;
    if (x == NULL) {
        printf("\nNOTHING TO EXTRACT");
        return x;
    }
    p = x;
    min = x->n;
    while (p->sibling != NULL) {
        if ((p->sibling)->n < min) {
            min = (p->sibling)->n;
            t = p;
            x = p->sibling;
        }
        p = p->sibling;
    }

    if (t == NULL && x->sibling == NULL) // binomial tree 裡面只有他一個
        H1 = NULL;
    else if (t == NULL) // binomial tree 裡面還有其他人，但最小值就是第一個點
        H1 = x->sibling;
    // else if (t->sibling == NULL) // 不知道在幹嘛的東西
    //     t = NULL;
    else
        t->sibling = x->sibling; // 把抽離最小值的地方前後補起來

    if (x->child != NULL) {
        new_head = REVERT_LIST(x->child);
    }
    else
        new_head = NULL;

    H = bin_HEAP_UNION(H1, new_head);
    printf("\n這邊有被跑到喔\n");
    return x;
}
 
struct node* REVERT_LIST(struct node* y) {
    if (y->sibling == NULL) {
        // 当到达最后一个节点时，返回它作为新的头节点
        return y;
    } else {
        // 递归反转剩余列表
        struct node* new_head = REVERT_LIST(y->sibling);
        // 将当前节点附加到其兄弟节点后面
        y->sibling->sibling = y;
        // 当前节点是新尾部，应该没有下一个节点
        y->sibling = NULL;
        return new_head;
    }
}

 
struct node* FIND_NODE(struct node* H, int k) {
    struct node* x = H;
    struct node* p = NULL;
    if (x->n == k) {
        p = x;
        return p;
    }
    if (x->child != NULL && p == NULL) {
        p = FIND_NODE(x->child, k);
    }
 
    if (x->sibling != NULL && p == NULL) {
        p = FIND_NODE(x->sibling, k);
    }
    return p;
}
 
int bin_HEAP_DECREASE_KEY(struct node* H, int i, int k) {
    int temp;
    struct node* p;
    struct node* y;
    struct node* z;
    p = FIND_NODE(H, i);
    if (p == NULL) {
        printf("\nINVALID CHOICE OF KEY TO BE REDUCED");
        return 0;
    }
    if (k > p->n) {
        printf("\nSORY!THE NEW KEY IS GREATER THAN CURRENT ONE");
        return 0;
    }
    p->n = k;
    y = p;
    z = p->parent;
    while (z != NULL && y->n < z->n) {
        temp = y->n;
        y->n = z->n;
        z->n = temp;
        y = z;
        z = z->parent;
    }
    printf("\nKEY REDUCED SUCCESSFULLY!");
}
 
int bin_HEAP_DELETE(struct node* H, int k) {
    struct node* np;
    if (H == NULL) {
        printf("\nHEAP EMPTY");
        return 0;
    }
 
    bin_HEAP_DECREASE_KEY(H, k, -1000);
    np = bin_HEAP_EXTRACT_MIN(H);
    if (np != NULL)
        printf("\nNODE DELETED SUCCESSFULLY");
}
 
// int main() {
//     int i, n, m, l;
//     struct node* p;
//     struct node* np;
//     char ch;
//     printf("\nENTER THE NUMBER OF ELEMENTS:");
//     scanf("%d", &n);
//     printf("\nENTER THE ELEMENTS:\n");
//     for (i = 1; i <= n; i++) {
//         scanf("%d", &m);
//         np = CREATE_NODE(m);
//         H = bin_HEAP_INSERT(H, np);
//     }
//     display(H, 0);
//     do {
//         printf("\nMENU:-\n");
//         printf(
//                 "\n1)INSERT AN ELEMENT\n2)EXTRACT THE MINIMUM KEY NODE\n3)DECREASE A NODE KEY\n 4)DELETE A NODE\n5)QUIT\n");
//         scanf("%d", &l);
//         switch (l) {
//         case 1:
//             do {
//                 printf("\nENTER THE ELEMENT TO BE INSERTED:");
//                 scanf("%d", &m);
//                 p = CREATE_NODE(m);
//                 H = bin_HEAP_INSERT(H, p);
//                 printf("\nNOW THE HEAP IS:\n");
//                 display(H, 0);
//                 printf("\nINSERT MORE(y/Y)= \n");
//                 fflush(stdin);
//                 scanf("%c", &ch);
//             } while (ch == 'Y' || ch == 'y');
//             break;
//         case 2:
//             do {
//                 printf("\nEXTRACTING THE MINIMUM KEY NODE");
//                 p = bin_HEAP_EXTRACT_MIN(H);
//                 if (p != NULL)
//                     printf("\nTHE EXTRACTED NODE IS %d", p->n);
//                 printf("\nNOW THE HEAP IS:\n");
//                 DISPLAY(H);
//                 printf("\nEXTRACT MORE(y/Y)\n");
//                 fflush(stdin);
//                 scanf("%c", &ch);
//             } while (ch == 'Y' || ch == 'y');
//             break;
//         case 3:
//             do {
//                 printf("\nENTER THE KEY OF THE NODE TO BE DECREASED:");
//                 scanf("%d", &m);
//                 printf("\nENTER THE NEW KEY : ");
//                 scanf("%d", &l);
//                 bin_HEAP_DECREASE_KEY(H, m, l);
//                 printf("\nNOW THE HEAP IS:\n");
//                 DISPLAY(H);
//                 printf("\nDECREASE MORE(y/Y)\n");
//                 fflush(stdin);
//                 scanf("%c", &ch);
//             } while (ch == 'Y' || ch == 'y');
//             break;
//         case 4:
//             do {
//                 printf("\nENTER THE KEY TO BE DELETED: ");
//                 scanf("%d", &m);
//                 bin_HEAP_DELETE(H, m);
//                 printf("\nDELETE MORE(y/Y)\n");
//                 fflush(stdin);
//                 scanf("%c", &ch);
//             } while (ch == 'y' || ch == 'Y');
//             break;
//         case 5:
//             printf("\nTHANK U SIR\n");
//             break;
//         default:
//             printf("\nINVALID ENTRY...TRY AGAIN....\n");
//         }
//     } while (l != 5);
// }

int main() {
    int N, M;
    struct node* p;
    struct node* np;
    scanf("%d %d", &N, &M);

    for (int i = 1; i <= M; i++) {
        int type, job_id, priority, printer_id;
        scanf("%d", &type);
        if (type == 1) {
            scanf("%d %d %d", &job_id, &priority, &printer_id);
            np = CREATE_NODE(priority, job_id);
            H = bin_HEAP_INSERT(H, np); // 這之後要換成不同的printer_id
            printf("\nNOW THE HEAP IS:\n");
            display(H, 0); // 用來debug
        }
        if (type == 2) {
            scanf("%d", &printer_id);
            p = bin_HEAP_EXTRACT_MIN(H);
            if (p != NULL) {
                printf("\nTHE EXTRACTED priority IS %d", p->n);
                printf("\nTHE EXTRACTED job_id IS %d", p->job_id);
            }
            printf("\nNOW THE HEAP IS:\n"); // debug
            display(H, 0);
        }
        if (type == 3) {
            // 這邊要合併 兩個 printer
            // H = bin_HEAP_UNION(H1, Hr);
        }
    }
    return 0;
}
