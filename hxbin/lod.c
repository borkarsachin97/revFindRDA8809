#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>



void read_fp_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // First block - has the "LOD" signature
    char signature[4];
    fread(signature, 1, 3, file);
    signature[3] = '\0'; // Null-terminate for safety

    if (strcmp(signature, "LOD") != 0) {
        printf("Invalid signature. Expected 'LOD', found '%s'\n", signature);
        fclose(file);
        return;
    }

    printf("Signature: %s\n", signature);

    // Read the 3 unknown bytes after the signature
    uint8_t unknown_bytes[3];
    fread(unknown_bytes, 1, 3, file);
    printf("Unknown bytes: %02x %02x %02x\n", unknown_bytes[0], unknown_bytes[1], unknown_bytes[2]);

    while (!feof(file)) {
        // Read 4-byte little-endian address
        uint32_t address;
        if (fread(&address, 1, 4, file) != 4) break;
        printf("Address: 0x%08x\n", address);

        // Read 2-byte little-endian size
        uint16_t size;
        if (fread(&size, 1, 2, file) != 2) break;
        printf("Data size: 0x%04x (%d bytes)\n", size, size);

        // Read the data of size "size"
        uint8_t *data = malloc(size);
        if (data == NULL) {
            perror("Memory allocation failed");
            fclose(file);
            return;
        }

        if (fread(data, 1, size, file) != size) {
            printf("Error reading data block.\n");
            free(data);
            break;
        }

        // Print the data
        printf("Data block:\n");
        print_hex(data, size);
        free(data);

        // For subsequent blocks, read 1 unknown byte before reading the address
        uint8_t unknown_byte;
        if (fread(&unknown_byte, 1, 1, file) != 1) break;
        printf("Unknown byte: %02x\n", unknown_byte);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    read_fp_file(argv[1]);

    return 0;
}

