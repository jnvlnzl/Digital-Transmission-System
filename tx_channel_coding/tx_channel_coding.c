// CHANNEL CODING: CRC-8 

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// INPUT: Sequence of Frame Bytes in HEX FROM: Framing
// OUTPUT: Sequence of Frame Bits as '0' and '1' TO: Line Coding 

#define MAX_SEQUENCE_NUMBER 8000 // Maximum size of sequence number
#define MAX_BINARY_SEQUENCE MAX_SEQUENCE_NUMBER * 8 // Maximum size of binary sequence
#define CRC8_POLYNOMIAL 0x07

// CRC-8 calculation function
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

// Function to convert hexadecimal string to bytes
void hex_string_to_bytes(const char *hex_string, uint8_t *bytes, size_t len) {
    for (size_t i = 0; i < len; i++) {
        sscanf(hex_string + 2*i, "%2hhx", &bytes[i]);
    }
}

int main() {
    char input[MAX_BINARY_SEQUENCE]; 
    char output[MAX_BINARY_SEQUENCE * 9]; 

    // Pipe operation
    if (fgets(input, sizeof(input), stdin) == NULL) {
        perror("Error reading input from stdin");
        return 1;
    }

    // Inidividual Testing
    //FILE* inputFile = fopen("tx_channel_coding_input.txt", "r");
    //if (inputFile == NULL) {
    //    perror("Error opening input file");
    //    return 1;
    //}

    //fclose(inputFile);

    // Remove trailing newline (if any)
    input[strcspn(input, "\n")] = '\0';

    // Convert hexadecimal string to bytes
    size_t len = strlen(input) / 2;
    uint8_t *bytes = malloc(len);
    hex_string_to_bytes(input, bytes, len - 2); // Exclude the FCS part

    // Calculate the CRC-8 for the byte sequence
    uint8_t crc = crc8(bytes, len - 2); // Exclude the FCS part

    // Convert each byte of the input (excluding FCS) to its binary representation
    size_t output_index = 0; 
    for (size_t i = 0; i < len - 2; i++) { // Exclude the FCS part
        uint8_t byte = bytes[i]; 
        for (int j = 7; j >= 0; j--) { 
            output[output_index++] = ((byte >> j) & 1) + '0'; 
        }
    }

    // Convert CRC byte to its binary representation
    for (int j = 7; j >= 0; j--) {
        output[output_index++] = ((crc >> j) & 1) + '0'; 
    }

    output[output_index] = '\0';

    // Print the sequence of frame bits
    printf("%s\n", output);

    free(bytes);

    return 0;

}
