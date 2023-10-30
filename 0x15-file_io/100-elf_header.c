#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <string.h>

void print_elf_header_info(Elf64_Ehdr *elf_header) {
    printf("Magic: ");
    for (int i = 0; i < EI_NIDENT; i++) {
        printf("%02x ", elf_header->e_ident[i]);
    }
    printf("\n");

    printf("Class: %d-bit\n", elf_header->e_ident[EI_CLASS] == ELFCLASS64 ? 64 : 32);
    printf("Data: %s-endian\n", elf_header->e_ident[EI_DATA] == ELFDATA2LSB ? "Little" : "Big");
    printf("Version: %d\n", elf_header->e_ident[EI_VERSION]);
    printf("OS/ABI: %d\n", elf_header->e_ident[EI_OSABI]);
    printf("ABI Version: %d\n", elf_header->e_ident[EI_ABIVERSION]);

    printf("Type: %d\n", elf_header->e_type);
    printf("Entry point address: 0x%lx\n", (unsigned long)elf_header->e_entry);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s elf_filename\n", argv[0]);
        return 98;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return 98;
    }

    Elf64_Ehdr elf_header;
    if (read(fd, &elf_header, sizeof(Elf64_Ehdr)) != sizeof(Elf64_Ehdr)) {
        fprintf(stderr, "Error reading ELF header\n");
        close(fd);
        return 98;
    }

    if (memcmp(elf_header.e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "Not an ELF file\n");
        close(fd);
        return 98;
    }

    print_elf_header_info(&elf_header);

    close(fd);
    return 0;
}
