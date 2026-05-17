#include <stdio.h>
#include <stdlib.h>

/*
 * 19 字符串分割器（类似 strtok_r）
 * 目标：实现线程安全的字符串分割函数，不依赖静态全局状态
 * 原型：char* strtok_r(char *str, const char *delim, char **saveptr)
 * 示例：输入 "hello,world test"，分隔符 ", " → 依次得到 hello、world、test
 */

/* 判断字符 c 是否在分隔符集合 delim 中 */
static int is_delim(char c, const char *delim) {
    while (*delim) {
        if (c == *delim) {
            return 1;
        }
        delim++;
    }
    return 0;
}

/* 线程安全版本：通过 saveptr 维护调用状态，不使用静态变量 */
char *strtok_r(char *str, const char *delim, char **saveptr) {
    char *start;

    if (str) {
        start = str;
    } else {
        start = *saveptr;
    }

    /* 跳过前导分隔符 */
    while (*start && is_delim(*start, delim)) {
        start++;
    }

    if (*start == '\0') {
        *saveptr = start; // 没有更多 token，更新 saveptr
        return NULL;
    }

    char *end = start;
    while (*end && !is_delim(*end, delim)) {
        end++;
    }

    if (*end) {
        *end = '\0'; // 终止当前 token
        *saveptr = end + 1; // 更新 saveptr 指向下一个 token 的起始位置
    } else {
        *saveptr = end; // 已经到达字符串末尾，更新 saveptr
    }

    return start; // 返回当前 token 的起始位置
}

int main(void) {
    char buf[] = "hello,world test";
    const char *delim = ", ";
    char *save = NULL;

    char *tok = strtok_r(buf, delim, &save);
    while (tok) {
        printf("%s\n", tok);
        tok = strtok_r(NULL, delim, &save);
    }
    return 0;
}
