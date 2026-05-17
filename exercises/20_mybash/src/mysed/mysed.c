#include "mysed.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

static char* string_ndup(const char* s, size_t n) {
    char* result = malloc(n + 1);
    if (!result) {
        return NULL;
    }
    memcpy(result, s, n);
    result[n] = '\0';
    return result;
}

int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    if (!cmd || cmd[0] != 's' || cmd[1] != '/') {
        return -1;
    }

    const char* old_start = cmd + 2;
    const char* old_end = strchr(old_start, '/');
    if (!old_end) {
        return -1;
    }

    const char* new_start = old_end + 1;
    const char* new_end = strchr(new_start, '/');
    if (!new_end) {
        return -1;
    }

    size_t old_len = old_end - old_start;
    size_t new_len = new_end - new_start;

    *old_str = string_ndup(old_start, old_len);
    if (!*old_str) {
        return -1;
    }

    *new_str = string_ndup(new_start, new_len);
    if (!*new_str) {
        free(*old_str);
        return -1;
    }

    return 0;
}

void replace_first_occurrence(char* str, const char* old, const char* new) {
    char* pos = strstr(str, old);
    if (pos) {
        size_t old_len = strlen(old);
        size_t new_len = strlen(new);
        size_t tail_len = strlen(pos + old_len);

        memmove(pos + new_len, pos + old_len, tail_len + 1);
        memcpy(pos, new, new_len);
    }
}

int __cmd_mysed(const char* rules, const char* str) {
    if (!rules || !str) {
        fprintf(stderr, "Error: NULL rules or str parameter\n");
        return 1;
    }

    char* old_str = NULL;
    char* new_str = NULL;

    if (parse_replace_command(rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }

    if (!old_str || !new_str) {
        fprintf(stderr, "Error: Failed to parse old/new strings from rules\n");
        free(old_str);
        free(new_str);
        return 1;
    }

    // 复制原始字符串，因为我们可能会修改它（避免修改输入参数）
    char line[MAX_LINE_LENGTH];
    strncpy(line, str, sizeof(line) - 1);
    line[sizeof(line) - 1] = '\0';  // 确保终止

    // 执行替换：在 line 中把 old_str 替换为 new_str（第一次出现）
    replace_first_occurrence(line, old_str, new_str);

    // 输出结果（建议加上换行，除非 str 本身带了）
    printf("%s\n", line);

    // 释放动态分配的内存
    free(old_str);
    free(new_str);

    return 0;
}