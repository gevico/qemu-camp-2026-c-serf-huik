#include "myfile.h"

void print_elf_type(uint16_t e_type) {
    const char *type_str;
    const char *enum_str;
    switch (e_type) {
        case ET_NONE:
            type_str = "Unknown";
            enum_str = "ET_NONE";
            break;
        case ET_REL:
            type_str = "Relocatable";
            enum_str = "ET_REL";
            break;
        case ET_EXEC:
            type_str = "Executable";
            enum_str = "ET_EXEC";
            break;
        case ET_DYN:
            type_str = "Shared Object/PIE";
            enum_str = "ET_DYN";
            break;
        case ET_CORE:
            type_str = "Unknown";
            enum_str = "ET_CORE";
            break;
        default:
            type_str = "Unknown";
            enum_str = "UNKNOWN";
            break;
    }
    printf("ELF Type: %s (%s) (0x%x)\n", type_str, enum_str, e_type);
}

int __cmd_myfile(const char *filename) {
    int fd;
    Elf64_Ehdr ehdr;

    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open file");
        return 1;
    }

    if (read(fd, &ehdr, sizeof(Elf64_Ehdr)) != sizeof(Elf64_Ehdr)) {
        perror("Failed to read ELF header");
        close(fd);
        return 1;
    }

    if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "Not a valid ELF file: %s\n", filename);
        close(fd);
        return 1;
    }

    print_elf_type(ehdr.e_type);
    close(fd);
    return 0;
}