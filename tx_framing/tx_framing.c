#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEQUENCE_NUMBER 8000 // Maximum size of sequence number
#define MAX_BINARY_SEQUENCE MAX_SEQUENCE_NUMBER * 8 // Maximum size of binary sequence

// INPUT: Sequence of 0s and 1s from Source Coding
// OUTPUT: Framed sequence in binary in HEX
// Preamble = FF
// Frame Type = 01
// Payload Size = 0002
// FCS = 0000

int main() {
    char input[MAX_BINARY_SEQUENCE]; // Buffer to store binary sequence
    char hexOutput[MAX_SEQUENCE_NUMBER * 2]; // Buffer to store hexadecimal output

    // Open the input file for reading
    FILE* inputFile = fopen("tx_input_framing.txt", "r");
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

    // Close the input file as it's no longer needed
    fclose(inputFile);

    // Remove trailing newline (if any)
    input[strcspn(input, "\n")] = '\0';
    
    char preamble[] = "FF";
    int sequence_number = 1;
    int frame_type = 1;
    int payload_size = 2;
    int fcs = 0;

    // Convert binary input to hexadecimal
    int len = strlen(input);
    for (int i = 0; i < len; i += 4) {
        int byte = 0;
        for (int j = 0; j < 4 && i + j < len; j++) {
            byte = (byte << 1) | (input[i + j] - '0');
        }
        sprintf(hexOutput + i / 4, "%X", byte);
    }

    // Open the output file for writing
    FILE* outputFile = fopen("tx_output_framing.txt", "w");
    if (outputFile == NULL) {
        perror("Error creating output file");
        return 1;
    }

    int length = strlen(hexOutput);
    
    // Iterate through the word, printing two characters at a time
    for (int i = 0; i < length; i += 2) {
        int remaining_chars = length - i;  // Calculate remaining characters
        
        fprintf(outputFile, "%s%04X%02d%04d", preamble, sequence_number, frame_type, payload_size);
        
        // Handle trailing spaces if fewer than payload_size characters remaining
        if (remaining_chars < 2) {
            for (int k = 0; k < 2 - remaining_chars; k++) {
                fprintf(outputFile, "00");  // Print spaces for missing characters
            }
        }
        
        // Print up to payload_size characters in hexadecimal
        for (int j = 0; j < 2 && i + j < length; j++) {
            fprintf(outputFile, "%c", hexOutput[i + j]);
        }
        
        fprintf(outputFile, "%04d", fcs); // Print FCS and newline
        
        sequence_number++;   
    }

    // Close the output file
    fclose(outputFile);

    return 0;
}
