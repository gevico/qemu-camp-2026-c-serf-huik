#include "doubly_circular_queue.h"

#include <stdlib.h>

// 头尾哨兵
static struct node tailsentinel;
static struct node headsentinel = {0, NULL, &tailsentinel};
static struct node tailsentinel = {0, &headsentinel, NULL};

static link head = &headsentinel;
static link tail = &tailsentinel;

link make_node(int data) {
    link p = calloc(1, sizeof(struct node));
    p->data = data;
    p->prev = NULL;
    p->next = NULL;
    return p;
}

void free_node(link p) {
    free(p);
}

link search(int key) {
    for (link cur = head->next; cur != tail; cur = cur->next){
        if (cur->data == key){
            return cur;
        }
    }
    return NULL;
}

void insert(link p) {
    p->next = head->next;
    p->prev = head;
    head->next->prev = p;
    head->next = p;
}

void delete(link p) {
    if (p == head || p == tail) return;

    p->prev->next = p->next;
    p->next->prev = p->prev;
    p->next = NULL;
    p->prev = NULL;
}

void traverse(void (*visit)(link)) {
    for (link cur = head->next; cur != tail; cur = cur->next){
        visit(cur);
    }
}

void destroy(void) {
    link cur = head->next;
    link next;
    while(cur != tail){
        next = cur->next;
        free_node(cur);
        cur = next;
    }
    head->next = tail;
    tail->prev = head;

    tail->next = NULL;
    head->prev = NULL;
}
