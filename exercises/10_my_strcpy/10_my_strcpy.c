#include <stdio.h>

int main() {
    char source[] = "Hello, World! This is a test string for my_strcpy function.\n";
    char destination[101];
    
	char *p_src = source;
    char *p_dest = destination;

    while (*p_src != '\0') {
        *p_dest = *p_src;
        p_dest++;
        p_src++;

    }

    *p_dest = '\0';
    
    printf("拷贝后的字符串: %s", destination);
    
    return 0;
}