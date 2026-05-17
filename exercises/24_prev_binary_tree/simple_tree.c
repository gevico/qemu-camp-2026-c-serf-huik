#include "simple_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

Queue* create_queue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue *q, TreeNode *tree_node) {
    QueueNode* new_node = malloc(sizeof(QueueNode));
    new_node->tree_node = tree_node;
    new_node->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
}

TreeNode* dequeue(Queue *q) {
     if (q->front == NULL) return NULL;

    QueueNode* old_front = q->front;
    TreeNode* temp = q->front->tree_node;
    q->front = q->front->next;

    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(old_front);
    return temp;
}

bool is_empty(Queue *q) {
    return q->front == NULL;
}

void free_queue(Queue *q) {
    while (!is_empty(q)) {
        dequeue(q);
    }
    free(q);
}

TreeNode* build_tree_by_level(int *level_order, int size) {
    if (size == 0) return NULL;
    TreeNode* root = malloc(sizeof(TreeNode));
    root->val = level_order[0];
    root->left = root->right = NULL;

    Queue* q = create_queue();
    enqueue(q, root);

    int i = 1;
    while (i < size) {
        TreeNode* parent = dequeue(q);

        // 左孩子
        if (i < size && level_order[i] != INT_MIN) {
            TreeNode* left = malloc(sizeof(TreeNode));
            left->val = level_order[i];
            left->left = left->right = NULL;
            parent->left = left;
            enqueue(q, left);
        }
        i++;

        // 右孩子
        if (i < size && level_order[i] != INT_MIN) {
            TreeNode* right = malloc(sizeof(TreeNode));
            right->val = level_order[i];
            right->left = right->right = NULL;
            parent->right = right;
            enqueue(q, right);
        }
        i++;
    }

    return root;
}

void preorder_traversal(TreeNode *root) {
    // 前序遍历：根 -> 左 -> 右
    if (root == NULL) return;
    printf("%d ", root->val);
    preorder_traversal(root->left);
    preorder_traversal(root->right);
}

void preorder_traversal_iterative(TreeNode *root) {
    if (root == NULL) return;

    TreeNode* stack[1000];
    int top = -1;

    stack[++top] = root;
    while (top >= 0) {
        TreeNode *node = stack[top--];
        printf("%d ", node->val);

        if (node->right) stack[++top] = node->right;
        if (node->left) stack[++top] = node->left;
    }
}

void free_tree(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
