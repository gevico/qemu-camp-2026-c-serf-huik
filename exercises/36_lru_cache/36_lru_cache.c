#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 16 LRU 缓存淘汰算法（哈希表 + 双向链表）
 *  - put(k,v)、get(k) 均为 O(1)
 *  - 容量满时淘汰最久未使用（LRU）的元素
 */

typedef struct LRUNode {
    int key;
    int value;
    struct LRUNode* prev;
    struct LRUNode* next;
} LRUNode;

typedef struct HashEntry {
    int key;
    LRUNode* node;
    struct HashEntry* next;
} HashEntry;

typedef struct {
    int capacity;
    int size;
    LRUNode* head; /* 最近使用（MRU） */
    LRUNode* tail; /* 最久未使用（LRU） */
    /* 简单链式哈希表 */
    size_t bucket_count;
    HashEntry** buckets;
} LRUCache;

static unsigned hash_int(int key) {
    /* 简单的整数哈希函数 */
    return (unsigned)key * 2654435761 % 4294967296;
}

static HashEntry* hash_find(LRUCache* c, int key, HashEntry*** pprev_next) {
    unsigned hash = hash_int(key);
    size_t idx = hash % c->bucket_count;
    HashEntry* prev = NULL;
    HashEntry* entry = c->buckets[idx];
    while (entry) {
        if (entry->key == key) {
            if (pprev_next) *pprev_next = prev ? &prev->next : &c->buckets[idx];
            return entry;
        }
        prev = entry;
        entry = entry->next;
    }
    if (pprev_next) *pprev_next = &c->buckets[idx];
    return NULL;
}

static void list_add_to_head(LRUCache* c, LRUNode* node) {
    node->prev = NULL;
    node->next = c->head;
    if (c->head) c->head->prev = node;
    c->head = node;
    if (!c->tail) c->tail = node; /* 首个节点 */
}

static void list_remove(LRUCache* c, LRUNode* node) {
    if (node->prev) node->prev->next = node->next;
    else c->head = node->next; /* 移除头节点 */

    if (node->next) node->next->prev = node->prev;
    else c->tail = node->prev; /* 移除尾节点 */
}

static void list_move_to_head(LRUCache* c, LRUNode* node) {
    list_remove(c, node);
    list_add_to_head(c, node);
}

static LRUNode* list_pop_tail(LRUCache* c) {
    if (!c->tail) return NULL;
    LRUNode* tail = c->tail;
    list_remove(c, tail);
    return tail;
}

/* LRU 接口实现 */
static LRUCache* lru_create(int capacity) {
    if (capacity <= 0) return NULL;
    LRUCache* c = malloc(sizeof(LRUCache));
    if (!c) return NULL;
    c->capacity = capacity;
    c->size = 0;
    c->head = NULL;
    c->tail = NULL;
    c->bucket_count = capacity * 2; /* 简单扩容策略 */
    c->buckets = calloc(c->bucket_count, sizeof(HashEntry*));
    if (!c->buckets) {
        free(c);
        return NULL;
    }
    return c;
}

static void lru_free(LRUCache* c) {
    if (!c) return;
    /* 释放链表节点 */
    LRUNode* node = c->head;
    while (node) {
        LRUNode* next = node->next;
        free(node);
        node = next;
    }
    /* 释放哈希表 */
    for (size_t i = 0; i < c->bucket_count; i++) {
        HashEntry* entry = c->buckets[i];
        while (entry) {
            HashEntry* next = entry->next;
            free(entry);
            entry = next;
        }
    }
    free(c->buckets);
    free(c);
}

static int lru_get(LRUCache* c, int key, int* out_value) {
    HashEntry* entry = hash_find(c, key, NULL);
    if (!entry) return 0; /* 未找到 */

    /* 移动到头部（最近使用） */
    list_move_to_head(c, entry->node);
    if (out_value) *out_value = entry->node->value;
    return 1; /* 成功 */
}

static void lru_put(LRUCache* c, int key, int value) {
    HashEntry* entry = hash_find(c, key, NULL);
    if (entry) {
        /* 更新现有节点值并移动到头部 */
        entry->node->value = value;
        list_move_to_head(c, entry->node);
        return;
    }

    /* 创建新节点 */
    LRUNode* new_node = malloc(sizeof(LRUNode));
    if (!new_node) return; /* 内存分配失败 */
    new_node->key = key;
    new_node->value = value;

    /* 添加到链表头部 */
    list_add_to_head(c, new_node);

    /* 添加到哈希表 */
    HashEntry* new_entry = malloc(sizeof(HashEntry));
    if (!new_entry) {
        /* 回退链表添加 */
        list_remove(c, new_node);
        free(new_node);
        return; /* 内存分配失败 */
    }
    new_entry->key = key;
    new_entry->node = new_node;
    unsigned hash = hash_int(key);
    size_t idx = hash % c->bucket_count;
    new_entry->next = c->buckets[idx];
    c->buckets[idx] = new_entry;

    c->size++;

    /* 超出容量，淘汰 LRU（尾节点） */
    if (c->size > c->capacity) {
        LRUNode* tail = list_pop_tail(c);
        if (tail) {
            /* 从哈希表中移除对应项 */
            HashEntry** pprev_next;
            HashEntry* tail_entry = hash_find(c, tail->key, &pprev_next);
            if (tail_entry) {
                *pprev_next = tail_entry->next; /* 从链表中移除 */
                free(tail_entry);
            }
            free(tail);
            c->size--;
        }
    }
}

/* 打印当前缓存内容（从头到尾） */
static void lru_print(LRUCache* c) {
    LRUNode* p = c->head;
    int first = 1;
    while (p) {
        if (!first) printf(", ");
        first = 0;
        printf("%d:%d", p->key, p->value);
        p = p->next;
    }
    printf("\n");
}

int main(void) {
    /* 容量 3：put(1,1), put(2,2), put(3,3), put(4,4), get(2), put(5,5) */
    LRUCache* c = lru_create(3);
    if (!c) {
        fprintf(stderr, "创建 LRU 失败\n");
        return 1;
    }

    lru_put(c, 1, 1); /* 缓存：1 */
    lru_put(c, 2, 2); /* 缓存：2,1 */
    lru_put(c, 3, 3); /* 缓存：3,2,1 (满) */
    lru_put(c, 4, 4); /* 淘汰 LRU(1)，缓存：4,3,2 */

    int val;
    if (lru_get(c, 2, &val)) {
        /* 访问 2：缓存：2,4,3 */
        (void)val; /* 演示无需使用 */
    }

    lru_put(c, 5, 5); /* 淘汰 LRU(3)，缓存：5,2,4 */

    /* 期望最终键集合：{2,4,5}，顺序无关。此处按最近->最久打印：5:5, 2:2, 4:4 */
    lru_print(c);

    lru_free(c);
    return 0;
}
