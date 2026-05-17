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

int main(int argc, char* argv[]) {
    const char* replace_rules = "s/unix/linux/";
    if (argc >= 2) {
        replace_rules = argv[1];
    }

    char line[MAX_LINE_LENGTH] = "unix is opensource. unix is free os.";

    char* old_str = NULL;
    char* new_str = NULL;

    if (parse_replace_command(replace_rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }

    replace_first_occurrence(line, old_str, new_str);
    fputs(line, stdout);

    free(old_str);
    free(new_str);
    return 0;
}
