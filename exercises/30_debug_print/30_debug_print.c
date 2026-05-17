#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

#ifdef DEBUG_LEVEL

#define DEBUG_PRINT(fmt, ...) do { \
    if (DEBUG_LEVEL == 1) printf("DEBUG: func=%s, line=%d\n", __func__, __LINE__); \
    else if (DEBUG_LEVEL == 2) printf("DEBUG: func=%s, line=%d, " fmt "\n", __func__, __LINE__, ##__VA_ARGS__); \
    else { printf("DEBUG: func=%s, line=%d, " fmt "\n", __func__, __LINE__, ##__VA_ARGS__); void *buffer[100]; int size = backtrace(buffer, 100); char **symbols = backtrace_symbols(buffer, size); if (symbols) { for (int i = 0; i < size; i++) printf("%s\n", symbols[i]); free(symbols); } } \
} while (0)

#else

#define DEBUG_PRINT(fmt, ...) do {} while (0)

#endif

























//! MUST BE ENSURE THE DEBUG_PRINT("x=%d", x) AT THE 48 LINE

// 测试代码
void test() {
    int x = 42;
    DEBUG_PRINT("x=%d", x);
}

int main() {
    test();
    return 0;
}