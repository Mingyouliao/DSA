#include <stdio.h>
#include <stdlib.h>

typedef long long ll;
#define MAX_PLAYERS 1000000 // 最多有10^6个玩家
#define MAX_SITUATION 500000 // 历史最多500000个事件 = 最多要輸出 500000 次攻擊

ll player_index[MAX_PLAYERS]; //裡面放的是第幾個player的rank是多少 例如：player_index[0] = 3 代表第一個player的rank是3
ll rank[MAX_PLAYERS]; //裡面放的是 現在rank多少 是哪個player 例如：rank[3] = 0 代表rank 3的player是第一個player
ll count[MAX_PLAYERS] = {0}; // 跟踪每个玩家的历史记录数量
ll N, T, M; // 玩家数、事件数、M值
ll total_situation_2 = 0; // 總共有多少個situations 2

typedef struct history_card{
    ll index; // 玩家索引
    ll power_up_times; // 玩家的攻擊增加值的次數
    ll power_up; // 玩家的增加值
    ll add_each; // 總共增加的值
    struct history_card *next; // 下一个历史
} history_card;

typedef struct power_playercard{
    ll index; // 玩家索引
    ll already_situation_2; // 玩家的已經有多少次situation 2
    ll power; // 玩家的戰鬥力
} power_playercard;

history_card* history[MAX_PLAYERS]; // 從新到舊的順序  
history_card* history_output[MAX_PLAYERS];  // 從舊到新的順序
history_card* history_tail[MAX_PLAYERS] = {NULL}; // 初始化为NULL
history_card* history_head[MAX_PLAYERS] = {NULL}; // 初始化为NULL
power_playercard power[MAX_PLAYERS];

void addHistory(ll playerIndex, ll powerUp) {
    history_card* newHistory = (history_card*)malloc(sizeof(history_card));
    newHistory->index = playerIndex;
    newHistory->power_up = powerUp;
    newHistory->next = NULL;

    if (history[playerIndex] != NULL) {
        newHistory->power_up_times = 1 + history[playerIndex]->power_up_times; // 更新玩家的增加值的次數
    } else {
        newHistory->power_up_times = 1;
    }
    newHistory->next = history[playerIndex]; // 新节点指向原链表头
    history[playerIndex] = newHistory; // 更新链表头为新节点
}

void addHistory_output(ll playerIndex, ll powerUp) {    // 這個是從舊到新的順序
    history_card* newHistory = (history_card*)malloc(sizeof(history_card));
    newHistory->index = playerIndex;
    newHistory->power_up = powerUp;
    newHistory->next = NULL;
    count[playerIndex]++; // 更新节点计数
    if (history_output[playerIndex] == NULL) {
        history_output[playerIndex] = newHistory; // 设置为头节点
        history_tail[playerIndex] = newHistory; // 同时也是尾节点 也是 M區間的尾节点
        history_head[playerIndex] = newHistory; // 同时也是头节点
        newHistory->add_each = powerUp;
    } else {
        history_tail[playerIndex]->next = newHistory; // 将新节点追加到原尾节点的后面
        newHistory->add_each = history_tail[playerIndex]->add_each + powerUp;
        if(count[playerIndex] > M){
            newHistory->add_each = newHistory->add_each - history_head[playerIndex]->power_up;
            history_head[playerIndex] = history_head[playerIndex]->next;
        }
        history_tail[playerIndex] = newHistory; // 更新尾节点为新节点
    }
}

void event1(ll attacker_index) {   // attack //param1 是哪個player發起攻擊，所以是player的index
    ll attacker_rank = player_index[attacker_index]; // 從player的index找到他的rank
    if (attacker_rank == 0) {
        return;
    }
    else{
        ll attacker = rank[attacker_rank]; // 從attacker的rank找到他的player_index，這行是去確保邏輯沒有問題，這個應該要跟attackter_index一樣。
        ll defender_rank = attacker_rank - 1; // 從attacker的rank找到defender的rank
        ll defender = rank[defender_rank]; // 從defender的rank找到他的player_index，所以我現在就可以從player_index[defender]裡面找到他的rank
        ll attacker_powerup = (total_situation_2 - power[attacker_rank].already_situation_2) * (N - attacker_rank - 1);

        power[attacker_rank].power += attacker_powerup;
        power[attacker_rank].already_situation_2 = total_situation_2; // 更新這個玩家已經有多少次situation 2

        ll defender_powerup = (total_situation_2 - power[defender_rank].already_situation_2) * (N - defender_rank - 1);
        power[defender_rank].power += defender_powerup;
        power[defender_rank].already_situation_2 = total_situation_2; // 更新這個玩家已經有多少次situation 2
        
        ll powerUp = power[defender_rank].power - power[attacker_rank].power; // 計算這次攻擊的能力值增加多少
        power[attacker_rank].power += powerUp; // 更新attacker的能力值變得跟defender一樣

        addHistory(attacker_index, powerUp);
        addHistory_output(attacker_index, powerUp);

        ll temp_player_insiderank = player_index[attacker]; 
        player_index[attacker] = player_index[defender]; 
        player_index[defender] = temp_player_insiderank;

        ll temp_rank_playerindex = rank[attacker_rank]; // player_index 的 attacker 的 就是要 attack 的 rank
        rank[attacker_rank] = rank[defender_rank];
        rank[defender_rank] = temp_rank_playerindex;

        ll temp3 = power[attacker_rank].index;
        power[attacker_rank].index = power[defender_rank].index;
        power[defender_rank].index = temp3;
    }
}

void event2() { // situation 2 的增加也要按照順序放進去history裡面
    total_situation_2++;
}

void event3_binarySearch(ll target) {
    ll left = 0; 
    ll right = N - 1;   // 這裡的N是玩家數量
    ll result = -1; // 初始化结果为 -1，表示未找到

    while (left <= right) {
        int mid = left + (right - left) / 2; //這邊是在找到最後一個大於等於target的玩家，所以mid就是在rank跑來跑去。
        ll power_change = (total_situation_2 - power[mid].already_situation_2) * (N - mid -1);
        power[mid].power += power_change;
        power[mid].already_situation_2 = total_situation_2; // 更新這個玩家已經有多少次situation 2

        if (power[mid].power >= target) {
            result = mid; // 更新找到的位置
            left = mid + 1; // 继续在右侧寻找可能存在的更后面的满足条件的位置
        } else {
            right = mid - 1;
        }
    }
    
    if (result == -1) {
        printf("0 0\n"); // 没有找到满足条件的玩家
    } else {
        printf("%lld %lld\n", result + 1, power[result].index + 1);
    }
}

void event4(ll target_player, ll m_i) {
    history_card *current = history[target_player]; // 使用局部指针遍历链表
    if (current == NULL){
        printf("0\n");
        return;
    } 

    if (m_i == 1) {
        printf("%lld\n", current->power_up);
    } else {
        printf("%lld\n", history_tail[target_player]->add_each);
    }
}
void printHistory() {
    for (ll i = 0; i < N; i++) { // 遍历每个玩家
        printf("Player %lld history:\n", i + 1);

        history_card* current = history[i];
        printf("  History:\n");
        while (current != NULL) { // 遍历并打印每个节点的index和power_up
            printf("    Index: %lld, Power up: %lld\n", current->index, current->power_up);
            current = current->next;
        }

        current = history_output[i];
        printf("  History Output:\n");
        while (current != NULL) { // 遍历并打印每个节点的index和power_up
            printf("    Index: %lld, Power up: %lld\n", current->index, current->power_up);
            current = current->next;
        }
    }
}
void freeHistory() {
    for (ll i = 0; i < N; i++) {
        history_card* current = history[i];
        while (current != NULL) {
            history_card* temp = current;
            current = current->next;
        }
    }
}

int main() {

    scanf("%lld %lld %lld", &N, &T, &M);

    for (ll i = 0; i < N; i++) {
        scanf("%lld", &power[i].power);
        power[i].index = i;
        power[i].already_situation_2 = 0;
        player_index[i] = i; // 裡面放的就是 每個 player 的rank，所以其實我可以直接用index去找到他的rank。
        rank[i] = i; // 裡面放的是 現在rank多少 是哪個player 例如：rank[3] = 0 代表rank 3的player是第一個player
        history[i] = NULL;
        history_output[i] = NULL;
    }

    for (ll i = 0; i < T; i++) {
        ll type, param1, param2;
        scanf("%lld", &type);
        switch (type) {
            case 1:
                scanf("%lld", &param1); // param1 是哪個player發起攻擊。 所以我要從給定哪個player發起攻擊，然後去找到他的rank 和 power。 index裡面應該要包的是player的rank
                event1(param1 - 1); // player的索引从0开始 
                break;
            case 2:
                event2();
                break;
            case 3:
                scanf("%lld", &param1);
                event3_binarySearch(param1);
                break;
            case 4:
                scanf("%lld %lld", &param1, &param2);
                event4(param1 - 1, param2);
                break;
        }
    }
    printf("\n");
    
    for (ll i = 0; i < N; i++) {
        history_card* current = history_output[i];
        history_card* total_output = history[i];
        if (current == NULL) {
            printf("0\n");
        }
        else{
            printf("%lld ", total_output->power_up_times);
            while (current != NULL) {
                printf("%lld ", current->power_up);
                current = current->next;
            }
            printf("\n");
        }
    }

    return 0;
}


// 在C语言中，player_card players[MAX_PLAYERS]; 和 player_card* players[MAX_PLAYERS]; 之间的主要差异在于它们存储的数据类型和用途：

// player_card players[MAX_PLAYERS];:

// 这是一个结构体数组，每个元素都是一个 player_card 类型的结构体。
// 这个数组直接在内存中分配了足够的空间来存储 MAX_PLAYERS 数量的 player_card 结构体。
// 当你访问这个数组的元素时，你直接操作的是这些结构体的实例，可以直接读取或修改它们的属性。
// 示例：players[0].power = 100; 直接设置第一个玩家的 power 属性为100。

// player_card* players[MAX_PLAYERS];:
// 这是一个指针数组，每个元素都是指向 player_card 类型结构体的指针。
// 这个数组在内存中为指针分配空间，但不为 player_card 结构体分配空间。你需要为每个玩家分别动态分配内存（通常使用 malloc），然后将每个结构体的地址赋给数组中的指针。
// 当你访问这个数组的元素时，你实际上是通过指针操作这些结构体，这允许你在需要时动态地添加或移除玩家。
// 示例：players[0] = malloc(sizeof(player_card)); players[0]->power = 100; 首先为第一个玩家分配内存，然后设置他的 power 属性为100。

// 使用场景
// 使用结构体数组（player_card players[MAX_PLAYERS];）更直接简单，适用于玩家数量固定，且在程序生命周期内不需要动态增减的情况。
// 使用指针数组（player_card* players[MAX_PLAYERS];）提供了更高的灵活性，允许动态管理每个玩家的数据。这在玩家数量会变化，或者每个玩家的数据在不同时间需要不同处理方式时非常有用。但是，使用这种方式需要更小心地管理内存，包括适时地使用 malloc 和 free。
// 根据你的具体需求和场景，选择最适合的数据结构来实现你的程序逻辑。