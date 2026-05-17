#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

void print_elf_type(uint16_t e_type) {
    const char *type_str;
    switch (e_type) {
        case ET_NONE:
            type_str = "Unknown";
            break;
        case ET_REL:
            type_str = "Relocatable";
            break;
        case ET_EXEC:
            type_str = "Executable";
            break;
        case ET_DYN:
            type_str = "Shared Object/PIE";
            break;
        case ET_CORE:
            type_str = "Unknown";
            break;
        default:
            type_str = "Unknown";
            break;
    }
    printf("ELF Type: %s (0x%x)\n", type_str, e_type);
}

static int ensure_object_file(const char *path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        return 0;
    }

    int status = system("gcc -Wall -Wextra -std=c99 -c 17_myfile.c -o 17_myfile.o 2>/dev/null");
    if (status == -1) {
        return -1;
    }
    return WIFEXITED(status) && WEXITSTATUS(status) == 0 ? 0 : -1;
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    char filepath[2][256] = {
        "./17_myfile.o",
        "./17_myfile",
    };

    if (ensure_object_file(filepath[0]) != 0) {
        fprintf(stderr, "Warning: failed to generate %s\n", filepath[0]);
    }

    int fd;
    Elf64_Ehdr ehdr;

    for (int i = 0; i < 2; i++) {
        fd = open(filepath[i], O_RDONLY);
        if (fd < 0) {
            continue;
        }

        if (read(fd, &ehdr, sizeof(ehdr)) != sizeof(ehdr)) {
            close(fd);
            continue;
        }

        if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
            close(fd);
            continue;
        }

        print_elf_type(ehdr.e_type);
        close(fd);
    }

    return 0;
}
