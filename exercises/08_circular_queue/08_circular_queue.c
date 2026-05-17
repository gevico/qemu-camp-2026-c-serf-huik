#include <stdio.h>
#include <stdbool.h>

#define MAX_PEOPLE 50

typedef struct {
    int id;
} People;

typedef struct {
    People data[MAX_PEOPLE];
    int head;
    int tail;
    int count;
} Queue;

void initQueue(Queue *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

int isEmpty(Queue *q) {
    return q->count == 0;
}

int isFull(Queue *q) {
    return q->count == MAX_PEOPLE;
}

int enqueue(Queue *q, People p) {
    if (isFull(q)) {
        return 0;
    }
    q->data[q->tail] = p;
    q->tail = (q->tail + 1) % MAX_PEOPLE;
    q->count++;
    return 1;
}

int dequeue(Queue *q, People *p) {
    if (isEmpty(q)) {
        return 0;
    }
    *p = q->data[q->head];
    q->head = (q->head + 1) % MAX_PEOPLE;
    q->count--;
    return 1;
}

int main() {
    Queue q;
    int total_people=50;
    int report_interval=5;

    initQueue(&q);
    
    // 入队
    for (int i = 1; i <= total_people; i++) {
        People p;
        p.id = i;
        enqueue(&q, p);
    }

    int count = 0;
    People p;

    while (q.count > 1) {
        dequeue(&q, &p);
        count++;
        if (count == report_interval) {
            // 出圈
            printf("淘汰: %d\n", p.id);
            count = 0;
        } else {
            // 没报到 k，重新入队
            enqueue(&q, p);
        }
    }
    
    printf("最后剩下的人是: %d\n", q.data[q.head].id);

    return 0;
}