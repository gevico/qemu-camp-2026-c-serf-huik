#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * URL参数解析器
 * 输入：包含http/https超链接的字符串
 * 输出：解析出所有的key-value键值对，每行显示一个
 */

int parse_url(const char* url) {
    int err = 0;

        // 查找 '?' 字符，获取查询字符串的起始位置
    const char* query_start = strchr(url, '?');
    if (!query_start) {
        fprintf(stderr, "❌ Error: No query parameters found in URL '%s'\n", url);
        err = 1;
        goto exit;
    }
    query_start++; // 跳过 '?'
    // 复制查询字符串到一个可修改的缓冲区
    char query[256];
    strncpy(query, query_start, sizeof(query) - 1);
    query[sizeof(query) - 1] = '\0'; // 确保字符串以 null 结尾
    // 使用 strtok 分割查询字符串
    char* token = strtok(query, "&");
    while (token) {
        char* equal_sign = strchr(token, '=');
        if (equal_sign) {
            *equal_sign = '\0'; // 将 '=' 替换为 '\0'，分割 key 和 value
            char* key = token;
            char* value = equal_sign + 1;
            printf("🔑 %s = %s\n", key, value);
        }
        token = strtok(NULL, "&");
    }

exit:
    return err;
}

int main() {
    const char* test_url = "https://cn.bing.com/search?name=John&age=30&city=New+York";

    printf("Parsing URL: %s\n", test_url);
    printf("Parameters:\n");

    parse_url(test_url);

    return 0;
}