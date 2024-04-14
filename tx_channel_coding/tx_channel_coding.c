// CHANNEL CODING: CRC-8 

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// INPUT: Sequence of Frame Bytes in HEX FROM: Framing
// OUTPUT: Sequence of Frame Bits as '0' and '1' TO: Line Coding 

#define MAX_SEQUENCE_NUMBER 8000 // Maximum size of sequence number
#define MAX_DATA_BLOCK_SIZE 38 // Maximum size of data block
#define MAX_BINARY_SEQUENCE MAX_SEQUENCE_NUMBER * 8 // Maximum size of binary sequence
#define CRC8_POLYNOMIAL 0x07 // CRC-8 Polynomial
#define FRAME_SIZE 18 // Size of each frame in hex characters

uint8_t crc8(uint8_t *data, size_t len) {
    uint8_t crc = 0;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t bit = 0; bit < 8; bit++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ CRC8_POLYNOMIAL;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

void hex_string_to_bytes(const char *hex_string, uint8_t *bytes, size_t len) {
    for (size_t i = 0; i < len; i++) {
        sscanf(hex_string + 2*i, "%2hhx", &bytes[i]);
    }
}

int main() {
    char input[MAX_BINARY_SEQUENCE]; 
    char output[MAX_BINARY_SEQUENCE * 9]; 

    if (fgets(input, sizeof(input), stdin) == NULL) {
        perror("Error reading input from stdin");
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';

    // Process each frame individually
    for (int i = 0; i < strlen(input); i += FRAME_SIZE) {
        char frame[FRAME_SIZE + 1];
        strncpy(frame, input + i, FRAME_SIZE);
        frame[FRAME_SIZE] = '\0';

        size_t len = strlen(frame) / 2;
        uint8_t *bytes = malloc(len);
        hex_string_to_bytes(frame, bytes, len - 2); // Exclude the FCS part

        uint8_t crc = crc8(bytes, len - 2); // Calculate CRC-8 for the frame

        size_t output_index = 0;
        for (size_t i = 0; i < len - 2; i++) {
            uint8_t byte = bytes[i];
            for (int j = 7; j >= 0; j--) {
                output[output_index++] = ((byte >> j) & 1) + '0';
            }
        }

        for (int j = 7; j >= 0; j--) {
            output[output_index++] = ((crc >> j) & 1) + '0';
        }

        output[output_index] = '\0';
        printf("%s", output); // Print the frame with the CRC

        free(bytes);
    }

    printf("\n");

    return 0;
}
