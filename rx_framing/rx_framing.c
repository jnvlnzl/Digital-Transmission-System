// CHANNEL CODING: CRC-8 

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// INPUT: Sequence of Frame Bits as '0' and '1' FROM: Line Decoding 
// OUTPUT: Sequence of Frame Bytes in HEX To: Channel Decoding

#define CRC8_POLYNOMIAL 0x07
#define MAX_BLOCK_SIZE 256
#define MAX_FRAME_SIZE 1024 // Maximum size of a frame
#define MAX_DATA_BLOCK_SIZE 32

// Structure representing a frame
typedef struct {
    char preamble[8];
    char header[16];
    char dataBlock[8]; // Placeholder for data block
    char fcs[8];
} Frame;

// Function to calculate CRC-8
unsigned char calculate_crc(const char *data, int size) {
    uint8_t crc = 0;
    size_t i, j;
    for (i = 0; i < size; ++i) {
        crc ^= data[i];
        for (j = 0; j < 8; ++j) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ CRC8_POLYNOMIAL;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

int main() {
    // Open the input file for reading
    FILE* inputFile = fopen("rx_framing_input.txt", "r");
    if (inputFile == NULL) {
        perror("Error opening rx_framing_input.txt");
        return 1;
    }

    // Create and open the output file for writing
    FILE* outputFile = fopen("rx_framing_output.txt", "w");
    if (outputFile == NULL) {
        perror("Error creating rx_framing_output.txt");
        fclose(inputFile);
        return 1;
    }

    // Read input from the file
    char input[MAX_FRAME_SIZE];
    if (fgets(input, sizeof(input), inputFile) == NULL) {
        perror("Error reading input from file");
        fclose(inputFile);
        fclose(outputFile);
        return 1;
    }

    // Close the input file as it's no longer needed
    fclose(inputFile);

    // Construct a frame structure
    Frame frame;

    // Extract preamble, header, and data block from input
    strncpy(frame.preamble, input, 8);
    strncpy(frame.header, input + 8, 16);
    strncpy(frame.dataBlock, input + 24, MAX_DATA_BLOCK_SIZE);

    // Calculate CRC-8 checksum for the data block
    unsigned char crc = calculate_crc(frame.dataBlock, strlen(frame.dataBlock));

    // Convert CRC to hexadecimal string and store in frame
    sprintf(frame.fcs, "%02X", crc);

    // Write the frame to the output file
    fprintf(outputFile, "Preamble: %s\n", frame.preamble);
    fprintf(outputFile, "Header: %s\n", frame.header);
    fprintf(outputFile, "Data Block: %s\n", frame.dataBlock);
    fprintf(outputFile, "FCS: %s\n", frame.fcs);

    // Close the output file
    fclose(outputFile);

    printf("Output written to rx_framing_output.txt\n");

    return 0;
}
