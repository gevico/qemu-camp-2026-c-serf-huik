#include <stdio.h>

int main() {
    char str[]="Don't ask what your country can do for you, but ask what you can do for your country.";
    int wordCount = 0;
    int inWord = 0;
	
    char *p = str;
    while (*p != '\0') {
        if (*p == ' ' || *p == '\n' || *p == '\t') {
            inWord = 0;
        } else if (!inWord) {
            inWord = 1;
            wordCount++;
        }
        p++;
    }
    
    printf("单词数量: %d\n", wordCount);
    
    return 0;
}