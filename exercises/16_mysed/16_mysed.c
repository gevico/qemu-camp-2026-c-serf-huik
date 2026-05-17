#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024

int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    if (cmd[0] != 's' || cmd[1] != '/') {
        return -1;
    }

    const char* first_slash = strchr(cmd, '/');
    if (!first_slash) {
        return -1;
    }

    const char* second_slash = strchr(first_slash + 1, '/');
    if (!second_slash) {
        return -1;
    }

    // Extract the old string
    *old_str = strndup(first_slash + 1, second_slash - first_slash - 1);
    if (!*old_str) {
        return -1;
    }

    // Extract the new string
    *new_str = strdup(second_slash + 1);
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

        // Shift the tail of the string to make room for the new string
        memmove(pos + new_len, pos + old_len, tail_len + 1); // +1 for null terminator
        // Copy the new string into place
        memcpy(pos, new, new_len);
    }
}

int main(int argc, char* argv[]) {
    const char* replcae_rules = "s/unix/linux/";

    char line[MAX_LINE_LENGTH] = {"unix is opensource. unix is free os."};

    char* old_str = NULL;
    char* new_str = NULL;
    
    if (parse_replace_command(replcae_rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }

    
    replace_first_occurrence(line, old_str, new_str);
    fputs(line, stdout);

    free(old_str);
    free(new_str);
    return 0;
}
