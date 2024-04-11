// CHANNEL CODING: CRC-8 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// INPUT: Sequence of Frame Bytes in HEX FROM: Framing
// OUTPUT: Sequence of Frame Bits as '0' and '1' TO: Line Coding 
// also an output: substitued fcs to tx_framing_output

// hi mali ito mali pagkaintindi ko wait wait

#define MAX_SEQUENCE_NUMBER 8000 // Maximum size of sequence number
#define MAX_BINARY_SEQUENCE MAX_SEQUENCE_NUMBER * 8 // Maximum size of binary sequence
#define POLYNOMIAL 0x07  // CRC-8 polynomial

// CRC-8 Calculation
unsigned char crc8(const unsigned char *data, int len) {
    unsigned char crc = 0;
    int i, j;

    for (i = 0; i < len; ++i) {
        crc ^= data[i];
        for (j = 0; j < 8; ++j) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ POLYNOMIAL;
            } else {
                crc <<= 1;
            }
        }
    }

    return crc;
}

// Function to convert a byte to binary string
void byte_to_binary_str(unsigned char byte, char* str) {
    for (int i = 7; i >= 0; --i) {
        str[7 - i] = (byte & (1 << i)) ? '1' : '0';
    }
    str[8] = '\0';
}

int main() {
    char input[MAX_BINARY_SEQUENCE]; // Buffer to store binary sequence

    // Open the input file for reading
    FILE* inputFile = fopen("tx_framing_input.txt", "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return 1;
    }

    // Read input from the file
    if (fgets(input, sizeof(input), inputFile) == NULL) {
        perror("Error reading input from file");
        fclose(inputFile);
        return 1;
    }

    fclose(inputFile);

    input[strcspn(input, "\n")] = '\0';
    
    char preamble[] = "FF";
    int sequence_number = 1;
    int frame_type = 1;
    int payload_size = 2;

    FILE* outputFile1 = fopen("tx_channel_coding_output_binary.txt", "w");
    FILE* outputFile2 = fopen("tx_channel_coding_output_framing.txt", "w");
    if (outputFile1 == NULL || outputFile2 == NULL) {
        perror("Error creating output files");
        return 1;
    }

    int len = strlen(input);

    char hexOutput[MAX_SEQUENCE_NUMBER * 2];
    for (int i = 0; i < len; i += 4) {
        int byte = 0;
        for (int j = 0; j < 4 && i + j < len; j++) {
            byte = (byte << 1) | (input[i + j] - '0');
        }
        sprintf(hexOutput + i / 4, "%X", byte);
    }

    int length = strlen(hexOutput);

    for (int i = 0; i < length; i += 2) {
        int remaining_chars = length - i; 
        
        char binary_preamble[9];
        byte_to_binary_str(strtol(preamble, NULL, 16), binary_preamble);
        
        char binary_sequence_number[17];
        byte_to_binary_str(sequence_number, binary_sequence_number);
        
        char binary_frame_type[9];
        byte_to_binary_str(frame_type, binary_frame_type);
        
        char binary_payload_size[9];
        byte_to_binary_str(payload_size, binary_payload_size);
        
        fprintf(outputFile1, "%s%s%s%s", binary_preamble, binary_sequence_number, binary_frame_type, binary_payload_size);
        
        for (int j = 0; j < 2 && i + j < length; j++) {
            char binary_hex[9];
            byte_to_binary_str(strtol(&hexOutput[i + j], NULL, 16), binary_hex);
            fprintf(outputFile1, "%s", binary_hex);
        }
        
        // Calculate CRC-8
        unsigned char frame_data[2];
        sscanf(hexOutput + i, "%2hhX", &frame_data[0]);
        sscanf(hexOutput + i + 2, "%2hhX", &frame_data[1]);
        unsigned char calculated_fcs = crc8(frame_data, 2);

        char binary_fcs[9]; 
        byte_to_binary_str(calculated_fcs, binary_fcs);
        fprintf(outputFile1, "%s", binary_fcs);
        
        fprintf(outputFile2, "%s%04d%02d%04d", preamble, sequence_number, frame_type, payload_size);

        for (int j = 0; j < 2 && i + j < length; j++) {
            fprintf(outputFile2, "%c", hexOutput[i + j]);
        }
        
        fprintf(outputFile2, "%s", binary_fcs);
        
        sequence_number++;   
    }

    fclose(outputFile1);
    fclose(outputFile2);

    return 0;
}
