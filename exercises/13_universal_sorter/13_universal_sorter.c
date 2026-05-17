#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*CompareFunc)(const void *, const void *);

int compareInt(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int compareFloat(const void *a, const void *b) {
    float diff = (*(float*)a - *(float*)b);
    return (diff > 0) ? 1 : ((diff < 0) ? -1 : 0);
}

int compareString(const void *a, const void *b) {
    return strcmp(*(char**)a, *(char**)b);
}

void sort(void *array, size_t n, size_t size, CompareFunc compare) {
    qsort(array, n, size, compare);
}

void processFile(const char *filename) {
    FILE *fin = fopen(filename, "r");
    if (!fin) {
        printf("错误: 无法打开文件 %s\n", filename);
        return;
    }

    int choice, n;
    if (fscanf(fin, "%d", &choice) != 1 || fscanf(fin, "%d", &n) != 1) {
        printf("错误: 文件 %s 格式不正确\n", filename);
        fclose(fin);
        return;
    }

    if (n > 20) n = 20;  // 最多支持20个元素

    printf("=== 处理数据来自: %s ===\n", filename);

    switch (choice) {
        case 1: {
            int arr[20];
            for (int i = 0; i < n; i++) {
                if (fscanf(fin, "%d", &arr[i]) != 1) {
                    printf("错误: 文件 %s 格式不正确\n", filename);
                    fclose(fin);
                    return;
                }
            }
            sort(arr, n, sizeof(int), compareInt);
            printf("排序结果 (整数): ");
            for (int i = 0; i < n; i++) {
                printf("%d ", arr[i]);
            }
            printf("\n");
            break;
        }
        case 2: {
            float arr[20];
            for (int i = 0; i < n; i++) {
                if (fscanf(fin, "%f", &arr[i]) != 1) {
                    printf("错误: 文件 %s 格式不正确\n", filename);
                    fclose(fin);
                    return;
                }
            }
            sort(arr, n, sizeof(float), compareFloat);
            printf("排序结果 (浮点数): ");
            for (int i = 0; i < n; i++) {
                printf("%.2f ", arr[i]);
            }
            printf("\n");
            break;
        }
        case 3: {
            char *arr[20];
            for (int i = 0; i < n; i++) {
                char buffer[100];
                if (fscanf(fin, "%s", buffer) != 1) {
                    printf("错误: 文件 %s 格式不正确\n", filename);
                    fclose(fin);
                    return;
                }
                arr[i] = strdup(buffer); // 动态分配字符串
            }
            sort(arr, n, sizeof(char*), compareString);
            printf("排序结果 (字符串): ");
            for (int i = 0; i < n; i++) {
                printf("%s ", arr[i]);
                free(arr[i]); // 释放动态分配的内存
            }
            printf("\n");
            break;
        }
        default:
            printf("错误: 文件 %s 中的选择项无效\n", filename);
    }

    fclose(fin);
}

int main() {
    processFile("int_sort.txt");
    processFile("float_sort.txt");

    return 0;
}