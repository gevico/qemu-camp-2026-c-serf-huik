#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_elf_type(uint16_t e_type) {
  const char *type_str;
  switch (e_type) {
    case ET_NONE: type_str = "No file type"; break;
    case ET_REL:  type_str = "Relocatable file"; break;
    case ET_EXEC: type_str = "Executable file"; break;
    case ET_DYN:  type_str = "Shared object file"; break;
    case ET_CORE: type_str = "Core file"; break;
    default:      type_str = "Unknown"; break;
  }
  printf("ELF Type: %s (0x%x)\n", type_str, e_type);
}

int main(int argc, char *argv[]) {
  char filepath[2][256] = {
    "./17_myfile.o",
    "./17_myfile",
  };

  int fd;
  Elf64_Ehdr ehdr;

  for (int i = 0; i < 2; i++) {
    printf("Reading ELF file: %s\n", filepath[i]);
    fd = open(filepath[i], O_RDONLY);
    if (fd < 0) {
      perror("Failed to open file");
      continue;
    }

    if (read(fd, &ehdr, sizeof(ehdr)) != sizeof(ehdr)) {
      perror("Failed to read ELF header");
      close(fd);
      continue;
    }

    if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
      fprintf(stderr, "Not a valid ELF file: %s\n", filepath[i]);
      close(fd);
      continue;
    }

    print_elf_type(ehdr.e_type);
    printf("\n");
    close(fd);
  }
  
  return 0;
}
