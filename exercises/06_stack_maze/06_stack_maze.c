#include <stdio.h>

#define MAX_ROW 5
#define MAX_COL 5

int maze[MAX_ROW][MAX_COL] = {
	0, 1, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
};

int visited[MAX_ROW][MAX_COL] = {0}; // 标记是否访问过

struct Point {
    int r, c;
};

struct Point stack[MAX_ROW * MAX_COL]; // 路径栈
int top = 0; // 栈顶指针

// 入栈
void push(struct Point p) {
    stack[top++] = p;
}

// 出栈
struct Point pop() {
    return stack[--top];
}

// 取栈顶
struct Point peek() {
    return stack[top - 1];
}


// 判断栈是否为空
int isEmpty() {
    return top == 0;
}

int main(void)
{
	// 起点
    struct Point start = {0, 0};
    struct Point end = {MAX_ROW - 1, MAX_COL - 1};

    push(start);
    visited[start.r][start.c] = 1;

    // 四个方向
    int dr[4] = {1, -1, 0, 0};
    int dc[4] = {0, 0, 1, -1};

    int found = 0;

    while (!isEmpty()) {
        struct Point cur = peek();

        if (cur.r == end.r && cur.c == end.c) {
            found = 1;
            break;
        }

        int moved = 0;
        for (int i = 0; i < 4; i++) {
            int nr = cur.r + dr[i];
            int nc = cur.c + dc[i];

            if (nr >= 0 && nr < MAX_ROW &&
                nc >= 0 && nc < MAX_COL &&
                maze[nr][nc] == 0 &&
                !visited[nr][nc]) {

                struct Point next = {nr, nc};
                push(next);
                visited[nr][nc] = 1;
                moved = 1;
                break; // 成功走一步就 break
            }
        }

        if (!moved) {
            pop(); // 四个方向都走不了，回退
        }
    }

    if (found) {
        // 倒序输出
        for (int i = top - 1; i >= 0; i--) {
            printf("(%d, %d)\n", stack[i].r, stack[i].c);
        }
    } else {
        printf("No Path!\n");
    }

	return 0;
}