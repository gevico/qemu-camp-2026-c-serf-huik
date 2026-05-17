#include "circular_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

Node* create_circular_list(int n) {
    if (n <= 0){
        return NULL;
    }

    Node* head = calloc(1, sizeof(Node));
    head->id = 1;
    Node* tail = head;

    for (int i = 2; i <= n; i++){
        Node* p = calloc(1, sizeof(Node));
        p->id = i;
        tail->next = p;
        tail = p;
    }

    tail->next = head;
    return head;
}

void free_list(Node* head) {
    if (head == NULL) return;

    Node* curr = head->next;
    while(curr != head){
        Node* temp = curr->next;
        free(curr);
        curr = temp;
    }

    free(head);
}
