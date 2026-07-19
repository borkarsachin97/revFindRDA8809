#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <input_payload.bin> <output_firmware.lod>\n", argv[0]);
        return 1;
    }

    // 1. Open the raw bare-metal MIPS binary payload (compiled starting at 0x81C00280)
    FILE *in_file = fopen(argv[1], "rb");
    if (!in_file) {
        perror("Error opening input payload");
        return 1;
    }

    // Calculate payload size
    fseek(in_file, 0, SEEK_END);
    uint32_t payload_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);

    // 2. Open the destination target firmware container file
    FILE *out_file = fopen(argv[2], "wb");
    if (!out_file) {
        perror("Error opening output file");
        fclose(in_file);
        return 1;
    }

    printf("[*] Packaging custom payload (%d bytes)...\n", payload_size);

    // --- SECTION 1: GLOBAL HEADER (6 Bytes) ---
    // Read by the bootloader parser as a 3-byte signature + 3-byte configuration tokens
    fwrite("LOD", 1, 3, out_file);
    uint8_t global_header_tokens[3] = {0x42, 0x02, 0x55}; // 'B', 0x02, 0x55
    fwrite(global_header_tokens, 1, 3, out_file);

    // --- SECTION 2: DESCRIPTOR BLOCK (Address + Size + 24-Byte Data) ---
    // Descriptor target block destination address mapping
    uint32_t block1_address = 0x01C000A0;
    // Data size: exactly 24 bytes (0x0018)
    uint16_t block1_size = 0x0018;

    fwrite(&block1_address, sizeof(uint32_t), 1, out_file);
    fwrite(&block1_size, sizeof(uint16_t), 1, out_file);

    // 24-byte structural matrix data spanning file offsets 0x0C to 0x23
    uint32_t descriptor_matrix[6] = {
        0x00000000, // 0x0C - 0x0F: Front structural padding
        0x81C00280, // 0x10 - 0x13: BootROM formal execution entry address target pointer
        0x00000000, // 0x14 - 0x17: Trailing zero padding
        0x00000000, // 0x18 - 0x1B: Trailing zero padding
        0x00000000, // 0x1C - 0x1F: Trailing zero padding
        0x00000000  // 0x20 - 0x23: Trailing zero padding up to the layout boundary edge
    };
    fwrite(descriptor_matrix, sizeof(uint32_t), 6, out_file);

    // Single byte delimiter read at offset 0x24 before the parser loops/breaks
    uint8_t block1_delimiter = 0x56;
    fwrite(&block1_delimiter, 1, 1, out_file);

    // --- SECTION 3: CORE EXECUTABLE DATA BLOCK (Targeted at 27C) ---
    // Map the block target placement to 0x81C0027C (4 bytes before true execution entry)
    uint32_t block2_address = 0x01C0027C;
    // Data size: Raw binary size + the 4-byte leading hardware cushion NOP
    uint16_t block2_size = (uint16_t)(payload_size + 4);

    fwrite(&block2_address, sizeof(uint32_t), 1, out_file);
    fwrite(&block2_size, sizeof(uint16_t), 1, out_file);

    // 1. Inject the hardware cushion NOP at exactly address 0x81C0027C
    uint32_t hardware_cushion_nop = 0x00000000;
    fwrite(&hardware_cushion_nop, sizeof(uint32_t), 1, out_file);

    // 2. Read the compiled machine code from the input binary and append it starting at 0x81C00280
    uint8_t *payload_buffer = malloc(payload_size);
    if (!payload_buffer) {
        perror("Memory allocation failure");
        fclose(in_file);
        fclose(out_file);
        return 1;
    }

    if (fread(payload_buffer, 1, payload_size, in_file) != payload_size) {
        perror("Error reading complete input binary payload");
        free(payload_buffer);
        fclose(in_file);
        fclose(out_file);
        return 1;
    }

    fwrite(payload_buffer, 1, payload_size, out_file);

    // Final file completion EOF verification marker token
    uint8_t file_eof_marker = 0x00;
    fwrite(&file_eof_marker, 1, 1, out_file);

    // Cleanup resources
    free(payload_buffer);
    fclose(in_file);
    fclose(out_file);

    printf("[+] Packaged firmware container image successfully created!\n");
    return 0;
}
