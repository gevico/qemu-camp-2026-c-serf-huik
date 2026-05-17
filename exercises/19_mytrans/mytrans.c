// mytrans.c
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "myhash.h"

void trim(char *str) {
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0)  // All spaces?
        return;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator
    *(end + 1) = 0;
}

int load_dictionary(const char *filename, HashTable *table, uint64_t *dict_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("无法打开词典文件");
        return -1;
    }

    char line[1024];
    char current_word[100] = {0};
    char current_translation[1024] = {0};
    int in_entry = 0;

    while (fgets(line, sizeof(line), file)) {
        trim(line);

        if (line[0] == '#') {
            // 新词条开始
            if (in_entry && current_word[0] && current_translation[0]) {
                // 插入上一个词条
                if (!hash_table_insert(table, current_word, current_translation)) {
                    fprintf(stderr, "插入词条失败: %s\n", current_word);
                } else {
                    (*dict_count)++;
                }
            }

            // 提取新单词
            strcpy(current_word, line + 1);  // 跳过 #
            current_translation[0] = 0;
            in_entry = 1;
        } else if (strncmp(line, "Trans:", 6) == 0) {
            // 翻译行
            strcpy(current_translation, line + 6);
            trim(current_translation);
        } else if (in_entry && line[0]) {
            // 继续翻译
            strcat(current_translation, " ");
            strcat(current_translation, line);
        }
    }

    // 插入最后一个词条
    if (in_entry && current_word[0] && current_translation[0]) {
        if (!hash_table_insert(table, current_word, current_translation)) {
            fprintf(stderr, "插入词条失败: %s\n", current_word);
        } else {
            (*dict_count)++;
        }
    }

    fclose(file);
    return 0;
}
