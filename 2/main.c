#include <stdio.h>
#include <stdlib.h>

typedef struct CardNode {
    int cardNumber; // 牌的編號
    struct CardNode *next; // 指向下一張牌的指標
} CardNode;

typedef struct Deck {
    CardNode *top;
    CardNode *bottom;
} Deck;

// Deck* 是指向Deck的指標
// Decl** 是指向Deck*的指標

Deck* createDeck() {
    Deck *deck = (Deck*)malloc(sizeof(Deck)); // 是一副牌
    if (!deck) return NULL;
    deck->top = NULL;
    deck->bottom = NULL;
    return deck;
}

void addCard(Deck *deck, int cardNumber) {
    CardNode *newCard = (CardNode*)malloc(sizeof(CardNode)); // 建構了一張牌，newcard的結構就是上面定義的CardNode
    if (!newCard) return;
    newCard->cardNumber = cardNumber; // 把newcard的cardNumber設為傳進來的cardNumber
    newCard->next = deck->top; // 把newcard的next設為目前deck的top，因為新的牌要放在最上面
    deck->top = newCard;  // 把目前deck的top的指標，指向newcard的指標，這樣newcard就成為了最上面的牌
    if (deck->bottom == NULL) {  // 如果目前deck的bottom是空的，代表加入牌前沒有牌，所以新加入的牌是第一張牌
        deck->bottom = newCard;
    }
}

void removeCard(Deck *deck) {
    if (deck->top == NULL) return;
    CardNode *temp = deck->top; 
    deck->top = deck->top->next; // 把目前deck的top指向下一張牌，因為我在定義CardNode的時候，next是指向下一張牌的指標，所以這邊用->next就是指向下一張牌，可以是NULL
    free(temp); // 釋放掉temp指向的記憶體
    if (deck->top == NULL) { // 如果目前deck的top是空的，代表這副牌已經沒有牌了，所以要把bottom也設為空
        deck->bottom = NULL; 
    }
}

void moveAll(Deck *source, Deck *destination) {
    if (source->top == NULL) return;
    if (destination->top == NULL) {
        destination->top = source->top; // 把destination的top指向source的top
        destination->bottom = source->bottom; // 把destination的bottom指向source的bottom
    } else {
        source->bottom->next = destination->top; // 把source的bottom的next指向destination的top
        destination->top = source->top; // 把destination的top指向source的top
    }
    source->top = NULL;    // 把source的top設為空
    source->bottom = NULL;  // 把source的bottom設為空
}

void mergeDecks(Deck *source, Deck *destination) {
    if (source->top == NULL) {
        return; // 如果源牌堆为空，则不进行任何操作。
    }
    if (destination->top == NULL) {
        destination->top = source->top;
        destination->bottom = source->bottom;
        source->top = NULL;
        source->bottom = NULL;
        return;
    }

    // 使用两个指针来分别遍历source和destination
    CardNode *currentSource = source->top;
    CardNode *currentDest = destination->top;

    // 为合并后的链表创建一个虚拟头节点，简化操作
    CardNode dummy;
    CardNode *tail = &dummy; // tail始终指向最后一个节点

    // 交替合并
    while (currentSource != NULL && currentDest != NULL) {
        tail->next = currentSource; // 将source的当前节点接到合并链表的尾部
        currentSource = currentSource->next; // 移动source的指针
        tail = tail->next; // 移动tail到新的尾部

        tail->next = currentDest; // 将destination的当前节点接到合并链表的尾部
        currentDest = currentDest->next; // 移动destination的指针
        tail = tail->next; // 移动tail到新的尾部
    }

    // 如果任一链表有剩余，直接连接到合并链表的尾部
    if (currentSource != NULL) {
        tail->next = currentSource;
    } else {
        tail->next = currentDest;
    }

    // 更新destination的top和bottom。top应该指向合并后链表的第一个真实节点，bottom应该更新为最后一个节点
    destination->top = dummy.next; // dummy.next指向合并后链表的第一个真实节点
    if (currentSource != NULL) {
        destination->bottom = source->bottom; // 如果source中还有剩余节点，source的底部即为合并后的底部
    }

    // 清空source牌堆
    source->top = NULL;
    source->bottom = NULL;
}

void printDecks(Deck **decks, int N) {
    for (int i = 0; i < N; i++) {
        CardNode *current = decks[i]->top;
        int count = 0;
        // 先计算这副牌中有多少张牌
        for (CardNode *node = current; node != NULL; node = node->next) {
            count++;
        }
        // 输出这副牌的数量
        printf("%d", count);
        // 输出这副牌中每张牌的编号
        while (current != NULL) {
            printf(" %d", current->cardNumber);
            current = current->next;
        }
        printf("\n");
    }
}

int main() {
    int N, M;
    scanf("%d %d", &N, &M);
    Deck **decks = (Deck**)malloc(N * sizeof(Deck*)); // decks是裝所有牌的陣列 N*Deck*的陣列
    for (int i = 0; i < N; i++) {
        decks[i] = createDeck();
    }

    for (int i = 0; i < M; i++) {
        int t, i, j, u; // t = 1, 2, 3, 4 分別代表四種操作, i = 目標的牌堆, j = 目標的牌堆, u = 要加入的牌 
        scanf("%d", &t);
        switch (t) {
            case 1:
                scanf("%d %d", &i, &u); // i = 目標的牌堆, u = 要加入的牌
                addCard(decks[i-1], u);
                break;
            case 2:
                scanf("%d", &i); // i = 目標的牌堆
                removeCard(decks[i-1]);
                break;
            case 3:
                scanf("%d %d", &i, &j); // i = 目標的牌堆, j = 目標的牌堆
                moveAll(decks[i-1], decks[j-1]);
                break;
            case 4:
                scanf("%d %d", &i, &j);// i = 目標的牌堆, j = 目標的牌堆
                mergeDecks(decks[i-1], decks[j-1]);
                break;
        }
    }

    printDecks(decks, N);

    // 释放资源
    for (int i = 0; i < N; i++) {
        CardNode *current = decks[i]->top;
        while (current != NULL) {
            CardNode *temp = current;
            current = current->next;
            free(temp);
        }
        free(decks[i]);
    }
    free(decks);

    return 0;
}
