#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define CHUNK_SIZE 0x1000 // 4096 bytes

// Function declarations
void print_hex(uint8_t *data, size_t size);

void print_hex(uint8_t *data, size_t size) {
    size_t i;
    for (i = 0; i < size; ++i) {
        if (i % 16 == 0 && i != 0) {
            printf("\n");
        }
        printf("%02x ", data[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file.bin>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    uint8_t buffer[CHUNK_SIZE];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, CHUNK_SIZE, file)) > 0) {
        printf("Bytes read: %04X\n", bytes_read); // Print the number of bytes read
        print_hex(buffer, bytes_read);
    }

    if (ferror(file)) {
        perror("Error reading file");
    }

    fclose(file);
    return 0;
}

