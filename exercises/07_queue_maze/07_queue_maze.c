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

int visited[MAX_ROW][MAX_COL] = {0};

struct Point {
    int r;
    int c;
};

struct Point parent[MAX_ROW][MAX_COL];
struct Point queue[MAX_ROW * MAX_COL];

int main(void)
{
    struct Point start = {0, 0};
    struct Point end = {MAX_ROW - 1, MAX_COL - 1};

    int dr[4] = {1, -1, 0, 0};
    int dc[4] = {0, 0, 1, -1};

    int head = 0;
    int tail = 0;

    queue[tail++] = start;
    visited[start.r][start.c] = 1;
    parent[start.r][start.c].r = -1;
    parent[start.r][start.c].c = -1;

    int found = 0;

    while (head < tail) {
        struct Point cur = queue[head++];

        if (cur.r == end.r && cur.c == end.c) {
            found = 1;
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nr = cur.r + dr[i];
            int nc = cur.c + dc[i];

            if (nr >= 0 && nr < MAX_ROW && nc >= 0 && nc < MAX_COL &&
                maze[nr][nc] == 0 && !visited[nr][nc]) {
                visited[nr][nc] = 1;
                parent[nr][nc] = cur;
                queue[tail++] = (struct Point){nr, nc};
            }
        }
    }

    if (!found) {
        printf("No path!\n");
        return 0;
    }

    struct Point path[MAX_ROW * MAX_COL];
    int path_len = 0;
    struct Point cur = end;

    while (cur.r != -1 && cur.c != -1) {
        path[path_len++] = cur;
        cur = parent[cur.r][cur.c];
    }

    for (int i = 0; i < path_len; i++) {
        printf("(%d, %d)\n", path[i].r, path[i].c);
    }

    return 0;
}
